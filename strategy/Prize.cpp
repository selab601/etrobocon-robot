#include "Prize.h"

using namespace drive;
using namespace detection;
using namespace device;
using namespace measurement;

namespace strategy{
    Prize::Prize(){
        linetrace_              = LineTrace::getInstance();
        lineDetection_          = new LineDetection();
        objectDetection_        = new ObjectDetection(25);
        timeMeasurement_        = new TimeMeasurement();
        distanceMeasurement_    = new DistanceMeasurement();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();
        straightRunning_        = new StraightRunning();
        train_                  = Train::getInstance();
        arm_                    = Arm::getInstance();
        gyro_                   = GyroSensor::getInstance();
        sonar_                  = SonarSensor::getInstance();

        strategySuccess_        = false;
        isLineTraceReset_       = false;
    }

    bool Prize::capture(){
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executeStrategy(strategyProcedure_[procedureNumber_])){
                lineTraceReset();
                distanceMeasurement_->reset();
                procedureNumber_++; //フラグを戻しておく
                hasExecutedPhase_ = false;
                isLineTraceReset_ = false;
                //ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        if(procedureNumber_ == strategyProcedure_.size()){//最後まで終わったら
            strategySuccess_ = true;
            return true;
        }
        return strategySuccess_;
    }

    //戦略手順を実行する
    bool Prize::executeStrategy(StrategyPhase strategyPhase){

        //baseAngleに対する車体の相対角度を保存
        static int diffAngle;

        //懸賞でソナーが隠れ使用できないときfalse
        static bool usable_sonar = false;

        switch(strategyPhase){

        //車体角度保存
        case StrategyPhase::INIT:
            bodyAngleMeasurement_->setBaseAngle();//始めの角度をセット
            polar_.setMaxPwm(20);
            gyro_->reset();
            lineTraceReset();
            return true;

        //現在の相対角度をセット
        case StrategyPhase::SET_DIFFANGLE1:
            diffAngle = bodyAngleMeasurement_->getRelative10();
            return true;

        //車体がbaseAngleと直角になるよう旋回
        case StrategyPhase::TURN_SOUTH:
            return polar_.bodyTurn(-900 - diffAngle, 20);

        //baseAngleまで旋回
        case StrategyPhase::TURN_EAST:
            return polar_.bodyTurn(0 - diffAngle, 20);

        //baseAngleと逆向き
        case StrategyPhase::TURN_WEST:
            return polar_.bodyTurn(-1800 - diffAngle, 20);

        //baseAngleまで旋回(信地旋回用の2倍速)
        case StrategyPhase::TURN_EAST2:
            return polar_.bodyTurn(0 - diffAngle, 40);

        //車体がbaseAngleと直角になるよう旋回
        case StrategyPhase::TURN_SOUTH2:
            return polar_.bodyTurn(-900 - diffAngle, 40);

        //懸賞まで10cm前進
        case StrategyPhase::APPROACH10CM:
            return polar_.runTo(100, 0);

        //方向修正を挟み再び16cm前進
        case StrategyPhase::APPROACH16CM:
            return polar_.runTo(160, 0);

        //方向修正を挟み再び16cm前進
        case StrategyPhase::APPROACH16CM_SONAR:
            if(sonar_->getDistance() < 4){
                return true;
            }
            return polar_.runTo(160, 0);

        //懸賞へのアプローチを一行でやる版
        case StrategyPhase::APPROACH_KENSHO:
            if(sonar_->getDistance() < 4){
                return true;
            }
            return polar_.runTo(250, -900);

        case StrategyPhase::ARM_DOWN:
            return arm_->down();

        //5cm前進し懸賞の下にアームを入れる
        case StrategyPhase::APPROACH5CM:
            return polar_.runTo(50, 0);

        //懸賞を持ち上げる, 1秒以上詰まったら一度下げる
        case StrategyPhase::PICKUP_PRIZE:
            startTimeMeasurement(1000);
            if(timeMeasurement_->getResult()){
                strategyProcedure_.insert(
                    strategyProcedure_.begin() + procedureNumber_ + 1,
                    StrategyPhase::YOISHO
                );
                return true;
            }
            return arm_->up();

        //引っかかった場合、アームを一旦下げる
        case StrategyPhase::YOISHO:
            if(arm_->setDegree(50)){
                strategyProcedure_.insert(
                    strategyProcedure_.begin() + procedureNumber_ + 1,
                    StrategyPhase::PICKUP_PRIZE
                );
                return true;
            }
            return false;

        //大きめに下がる（旋回時に懸賞が台に当たらないように）
        case StrategyPhase::BACK1:
            polar_.back(true);
            polar_.setMaxPwm(20);
            return polar_.runTo(320, 1800);

        //現在の相対角度をセット, 懸賞でソナーが隠れてないか検知
        case StrategyPhase::SET_DIFFANGLE2:
            polar_.back(false);
            diffAngle = bodyAngleMeasurement_->getRelative10();
            if(!objectDetection_->getResult()){
                usable_sonar = true;
                ev3_speaker_play_tone(880, 1000);
            }
            else{
                ev3_speaker_play_tone(440, 1000);
            }
            return true;

        //出口の新幹線を待つ
        case StrategyPhase::STOP_EXIT:
            straightRunning_->run(0);
            gyro_->reset();
            //ソナーが使用可能かつソナーに反応があったとき
            if (usable_sonar && objectDetection_->getResult()){
                strategyProcedure_.insert(
                        strategyProcedure_.begin() + procedureNumber_ + 1,
                        StrategyPhase::WAIT_1_SEC);
                ev3_speaker_play_tone(700, 2000);
                return true;
            }
            //新幹線がいないと分かるとき(時間で)
            else if (!train_->atExit()){
                return true;
            }
            return false;

        //通り過ぎてから1秒間待つ
        case StrategyPhase::WAIT_1_SEC:
            startTimeMeasurement(1000);
            straightRunning_->run(0);
            ev3_speaker_play_tone(700, 2000);
            return timeMeasurement_->getResult();

        //土俵を出る, 最初は勢い良く
        case StrategyPhase::LEAVE_AREA1:
            distanceMeasurement_->start(400);
            straightRunning_->run(30);
            return distanceMeasurement_->getResult();

        //最後は転倒しづらいよう減速
        case StrategyPhase::LEAVE_AREA2:
            //polar_.centerPivot(false);
            distanceMeasurement_->start(80);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //転倒したとき対策
        case StrategyPhase::RECOVER:
            distanceMeasurement_->start(20);
            straightRunning_->run(50);
            return distanceMeasurement_->getResult();

        //転倒したとき対策:ジャイロでコケ判定
        //現状走行中のジャイロ値がとんでもないため、使えないみたいです
        case StrategyPhase::RECOVER_GYRO:
            //一秒静止後、コケ判定
            if (gyro_->getAngle() > 15){
                distanceMeasurement_->start(20);
                straightRunning_->run(50);
                ev3_speaker_play_tone(440, 2000);
                return distanceMeasurement_->getResult();
            }
            ev3_speaker_play_tone(140, 2000);
            return true;

        //台に接近
        case StrategyPhase::APPROACH12CM:
            polar_.setMaxPwm(20);
            return polar_.runTo(120, 0);
        
        //懸賞を置く
        case StrategyPhase::PUTDOWN_PRIZE:
            return arm_->normal();
        
        //ラインまでバック(安定のため距離条件追加)
        case StrategyPhase::BACKTOLINE:
            distanceMeasurement_->start(50);
            polar_.back(true);
            polar_.runTo(300, 1800);
            return distanceMeasurement_->getResult() && lineDetection_->getResult();

        //現在の相対角度をセット, バック走行解除
        case StrategyPhase::SET_DIFFANGLE4:
            polar_.back(false);
            diffAngle = bodyAngleMeasurement_->getRelative10();
            polar_.centerPivot(false);
            return true;

        //ライン復帰(信地旋回で不要に)
        case StrategyPhase::RETURN_LINE:
            distanceMeasurement_->start(50);
            straightRunning_->run(20);
            return distanceMeasurement_->getResult();
            //return polar_.runTo(70, 0);

        case StrategyPhase::TEST:
            arm_->down();
            straightRunning_->run(20);
            if (sonar_->getDistance() < 4){
                ev3_speaker_play_tone(70, 1000);
                straightRunning_->run(0);
                return true;
            }
            else{
                ev3_speaker_play_tone(700, 1000);
            }
            return false;

        case StrategyPhase::EXIT:
            return true;

        default: return false;
        }
        return false;
    }



    //時間検知をまとめたもの
    void Prize::startTimeMeasurement(int time,bool flagChange){
        if(!hasExecutedPhase_){
            timeMeasurement_->setBaseTime();
            timeMeasurement_->setTargetTime(time);
            if(flagChange){
                hasExecutedPhase_ = true;
            }
        }
    }

    void Prize::lineTraceReset(){
        if(!isLineTraceReset_){
            linetrace_->reset();
            isLineTraceReset_ = true;
        }
    }

}
