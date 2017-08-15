#include "ETSumoNeo.h"

using namespace drive;
using namespace device;
using namespace detection;
using namespace measurement;

namespace strategy{
    ETSumoNeo::ETSumoNeo(){
        linetrace_              = LineTrace::getInstance();
        objectDetection_        = new ObjectDetection();
        rightAngledDetection_   = new RightAngledDetection();
        timeMeasurement_        = new TimeMeasurement();
        distanceMeasurement_    = new DistanceMeasurement();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();
        straightRunning_        = new StraightRunning();
        train_                  = Train::getInstance();
        arm_                    = Arm::getInstance();

        hasExecutedPhase_       = false;
        strategySuccess_        = false;
        isLineTraceReset_       = false;
        colorDetected_          = false;

        // 黒->白の直角だけを判定する
        rightAngledDetection_->setDetectBlack(false);
        rightAngledDetection_->setDetectWhite(true);
    }

    bool ETSumoNeo::capture(){
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executeStrategy(strategyProcedure_[procedureNumber_])){
                lineTraceReset();
                distanceMeasurement_->reset();
                forcingOut_.reset();
                procedureNumber_++;
                hasExecutedPhase_ = false;//フラグを戻しておく
                isLineTraceReset_ = false;
                colorDetected_ = false;
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
    bool ETSumoNeo::executeStrategy(StrategyPhase strategyPhase){
        static int diffDegree10 = 0;
        switch(strategyPhase){

        //車体角度保存
        case StrategyPhase::INIT:
            bodyAngleMeasurement_->setBaseAngle();//始めの角度をセット
            return true;

        //土俵を向くまでライントレース
        case StrategyPhase::LINE_TRACE:
            linetrace_->setMaxPwm(50);
            distanceMeasurement_->start(1200);
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setEdge(LineTraceEdge::RIGHT);
            linetrace_->runCurve(400);
            //距離検知or車体角度が土俵を向いたらtrue
            return distanceMeasurement_->getResult() || bodyAngleMeasurement_->getResult() >= 180;

        //すこしライントレース
        case StrategyPhase::LINE_TRACE_LITTLE:
            distanceMeasurement_->start(60);
            linetrace_->setPid(LineTracePid::RETURN);
            linetrace_->run(20,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        // 入り口で新幹線を検知するまで停止
        case StrategyPhase::STOP_ENTRY:
            straightRunning_->run(0);
            // 新幹線が入り口にいないと分かるとき(時間で)
            if (!train_->atEntrance()){
                ev3_speaker_play_tone(300, 4);
                return true; // TODO: 入り口でも見なくて大丈夫か実験
            }
            else{
                ev3_speaker_play_tone(900, 4);
            }
            if (objectDetection_->getResult()){
                train_->setEntrance();  // 入り口で新幹線を見たことを知らせる
                strategyProcedure_.insert( // 通過するのを待つ
                        strategyProcedure_.begin() + procedureNumber_ + 1,
                        StrategyPhase::WAIT_1_SEC);
                ev3_speaker_play_tone(700, 2000);
                return true;
            }
            return false;

        // 中央で新幹線がいなくなるまで停止
        case StrategyPhase::STOP_CENTER:
            straightRunning_->run(0);
            // 新幹線がいないと分かるとき(時間で)
            if (!train_->atCenter()){
                return true;
            }
            else{
                ev3_speaker_play_tone(900, 4);
            }
            if (objectDetection_->getResult()){
                train_->setCenter();  // 中央で新幹線を見たことを知らせる
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

        //登壇後の動作を安定させるため少し旋回
        case StrategyPhase::TURN_LITTLE:
            return polar_.bodyTurn(3, 30);

        //登壇走行
        case StrategyPhase::CLIMB:
            polar_.setMaxPwm(50);
            polar_.back(false);
            return polar_.runToXY(450, 25);

        case StrategyPhase::LINE_RETURN:
            lineTraceReset();
            distanceMeasurement_->start(140);
            linetrace_->setTarget(0.5);
            linetrace_->setMaxPwm(20);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            linetrace_->setPid(0.09, 0, 4.3605);
            linetrace_->run();
            return distanceMeasurement_->getResult();

        case StrategyPhase::TO_RIGHT_ANGLE:
            lineTraceReset();
            linetrace_->setTarget(0.5);
            linetrace_->setMaxPwm(20);
            linetrace_->setPid(LineTracePid::VERY_FAST);
            linetrace_->run();
            return rightAngledDetection_->getResult(4.5);

        case StrategyPhase::STABILIZE:
            lineTraceReset();
            distanceMeasurement_->start(40);
            linetrace_->setTarget(0.5);
            linetrace_->setMaxPwm(20);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run();
            return distanceMeasurement_->getResult();

        case StrategyPhase::TO_CENTER:
            polar_.back(true);
            polar_.setMaxPwm(20);
            polar_.centerPivot(true);
            if (polar_.runTo(118, -1750 - bodyAngleMeasurement_->getRelative10()) ){
                diffDegree10 = bodyAngleMeasurement_->getRelative10();
                return true;
            }
            return false;

        // 車体の角度をラインに並行にする
        case StrategyPhase::TURN_STRAIGHT:
            polar_.centerPivot(true);
            polar_.back(false);
            return polar_.bodyTurn(-diffDegree10 - 10, 15); // 足りない事が多いから1°多く曲げる

        case StrategyPhase::ARM_UP:
            return arm_->setDegree(60);

        case StrategyPhase::ARM_HOLD:
            return arm_->setDegree(35);

        case StrategyPhase::ARM_NORMAL:
            return arm_->normal();

        case StrategyPhase::TO_BLOCK1:
            arm_->setDegree(35);    // 高速化のため同時にアームを上げる
            return polarToBlock(557);

        case StrategyPhase::BACK1:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800 +557 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK2:
            return polarToBlock(1243 +SLIP_DEGREE10); // タイヤが滑ってだんだん右にずれるので応急処置

        case StrategyPhase::BACK2:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800+SLIP_DEGREE10 +1243 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK3:
            return polarToBlock(2357 + SLIP_DEGREE10*2); // タイヤが滑ってだんだん右にずれるので応急処置

        case StrategyPhase::BACK3:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800 +2357+SLIP_DEGREE10*2 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK4:
            return polarToBlock(3043 + SLIP_DEGREE10*3); // タイヤが滑ってだんだん右にずれるので応急処置で+15

        case StrategyPhase::BACK4:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800 +3043+SLIP_DEGREE10*3 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_LINE_RETURN:
            arm_->normal();     // 高速化のため同時にアームを戻す
            polar_.back(true);
            return polar_.runTo(130, 1800 +SLIP_DEGREE10*3- bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_LEFTEDGE:
            polar_.back(false);
            return polar_.bodyTurn(150, 30);

        case StrategyPhase::FORCING_OUT:
            if (forcingOut_.run(30, isRight_)){
                blockPlaceNum_++;
                return true;
            }
            return false;

        case StrategyPhase::SEE_BLOCK:
            {
            colorid_t color = colorDetection_.getResult();
            if(!colorDetected_){
                colorDetected_ = true;
                return false;
            }
            isRight_ = (color == blockPlaceColors_[blockPlaceNum_]);
            // 奇数番号だったら寄り切りと押出しが左右逆になる
            if (blockPlaceNum_ % 2){
                isRight_ = !isRight_;
            }

            // 寄り切りのときは控えめにする
            if (color == blockPlaceColors_[blockPlaceNum_]){
                forcingOut_.setDegree(22);
            }
            else{
                forcingOut_.setDegree(35);
            }
            }
            return true;

        //検知した後すこし待つ
        case StrategyPhase::WAIT_2_SEC:
            startTimeMeasurement(2000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        case StrategyPhase::NEXT_STAGE:
            polar_.setMaxPwm(50);
            polar_.back(false);
            return polar_.runTo(360, 10 - bodyAngleMeasurement_->getRelative10());

        default: return false;
        }
        return false;
    }



    //時間検知をまとめたもの
    void ETSumoNeo::startTimeMeasurement(int time,bool flagChange){
        if(!hasExecutedPhase_){
            timeMeasurement_->setBaseTime();
            timeMeasurement_->setTargetTime(time);
            if(flagChange){
                hasExecutedPhase_ = true;
            }
        }
    }

    void ETSumoNeo::lineTraceReset(){
        if(!isLineTraceReset_){
            linetrace_->reset();
            isLineTraceReset_ = true;
        }
    }

    bool ETSumoNeo::polarToBlock(int degree10){
        polar_.setTurnPwm(25);
        polar_.back(false);
        polar_.setMaxPwm(30);
        if (polar_.runTo(CENTER_TO_BLOCK_LENGTH, degree10 - bodyAngleMeasurement_->getRelative10()) ){
            colorDetected_ = false;
            return true;
        }
        return false;
    }

}
