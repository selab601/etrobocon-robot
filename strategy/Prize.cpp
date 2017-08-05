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
                ev3_speaker_play_tone ( 500, 100);//音を出す
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
            return true;

        //現在の相対角度をセット
        case StrategyPhase::SET_DIFFANGLE1:
            diffAngle = bodyAngleMeasurement_->getRelative10();
            return true;

        //車体がbaseAngleと直角になるよう旋回
        case StrategyPhase::TURN_SOUTH:
            return polar_.bodyTurn(-900 - diffAngle, 20);

        //10cm前進、補正後もう一回呼びます
        case StrategyPhase::APPROACH1:
            arm_->down();
            return polar_.runTo(100, 0);
            //return polar_.runTo(200, -900);

        case StrategyPhase::ARM_DOWN:
            return arm_->down();

        //5cm前進し懸賞の下にアームを入れる
        case StrategyPhase::ACCESS1:
            return polar_.runTo(50, 0);

        //懸賞を持ち上げる, 2秒以上詰まったら一度下げる
        case StrategyPhase::PICKUP_PRIZE:
            startTimeMeasurement(1000);
            if(timeMeasurement_->getResult()){
                strategyProcedure_.insert(
                    strategyProcedure_.begin() + procedureNumber_ + 1,
                    StrategyPhase::YOISHO
                );
                return true;
            }
            return arm_->setDegree(70);

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
                ev3_speaker_play_tone(2000, 1000);
            }
            else{
                ev3_speaker_play_tone(60, 1000);
            }
            return true;

        //baseAngleまで旋回
        case StrategyPhase::TURN_EAST:
            return polar_.bodyTurn(0 - diffAngle, 20);

        case StrategyPhase::TURN_WEST:
            return polar_.bodyTurn(-1800 - diffAngle, 20);

        //出口の新幹線を待つ
        case StrategyPhase::STOP_EXIT:
            straightRunning_->run(0);
            // 新幹線がいないと分かるとき(時間で)
            if (!train_->atExit()){
                return true;
            }
            else{
                ev3_speaker_play_tone(900, 4);
            }
            if (usable_sonar && objectDetection_->getResult()){
                strategyProcedure_.insert( // 通過するのを待つ
                        strategyProcedure_.begin() + procedureNumber_ + 1,
                        StrategyPhase::WAIT_1_SEC);
                ev3_speaker_play_tone(700, 2000);
                return true;
            }
            return false;

        //通り過ぎてから1秒間待つ
        case StrategyPhase::WAIT_1_SEC:
            startTimeMeasurement(1000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //土俵を出る
        case StrategyPhase::LEAVE_AREA1:
            //polar_.setMaxPwm(40);
            //return polar_.runTo(500, 0);
            distanceMeasurement_->start(300);
            straightRunning_->run(30);
            return distanceMeasurement_->getResult();

        //土俵を出る
        case StrategyPhase::LEAVE_AREA2:
            //polar_.setMaxPwm(40);
            //return polar_.runTo(500, 0);
            polar_.centerPivot(false);
            distanceMeasurement_->start(200);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //土俵を出る
        case StrategyPhase::LEAVE_AREA_BACK:
            polar_.back(true);
            polar_.centerPivot(false);
            polar_.setMaxPwm(40);
            return polar_.runTo(500, 1800);
            //distanceMeasurement_->start(500);
            //straightRunning_->run(-30);
            //return distanceMeasurement_->getResult();

        //懸賞の重さで前にコケる可能性があるので、一秒待った後ちょっと前進
        case StrategyPhase::RECOVER:
            return polar_.runTo(40, 0);

        //台に接近
        case StrategyPhase::APPROACH2:
            polar_.setMaxPwm(20);
            return polar_.runTo(80, 0);
        
        //懸賞を置く
        case StrategyPhase::PUTDOWN_PRIZE:
            return arm_->normal();
        
        //ラインまでバック(安定のため距離条件追加)
        case StrategyPhase::BACK2:
            distanceMeasurement_->start(50);
            polar_.back(true);
            polar_.runTo(150, 1800);
            return distanceMeasurement_->getResult() && lineDetection_->getResult();

        //現在の相対角度をセット, バック走行解除
        case StrategyPhase::SET_DIFFANGLE4:
            polar_.back(false);
            diffAngle = bodyAngleMeasurement_->getRelative10();
            polar_.centerPivot(false);
            return true;

        //baseAngleまで旋回(信地旋回用の2倍速)
        case StrategyPhase::TURN_EAST2:
            return polar_.bodyTurn(0 - diffAngle, 40);

        //ライン復帰
        case StrategyPhase::RETURN_LINE:
            distanceMeasurement_->start(50);
            straightRunning_->run(20);
            return distanceMeasurement_->getResult();
            //return polar_.runTo(70, 0);

        case StrategyPhase::EXIT:
            return true;

        case StrategyPhase::ARM:
            arm_->up();
            return false;

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
