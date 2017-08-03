#include "Prize.h"

using namespace drive;
using namespace device;
using namespace measurement;

namespace strategy{
    Prize::Prize(){
        linetrace_              = LineTrace::getInstance();
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

        static int diffAngle;

        switch(strategyPhase){

        //車体角度保存
        case StrategyPhase::INIT:
            bodyAngleMeasurement_->setBaseAngle();//始めの角度をセット
            return true;

        case StrategyPhase::APPROACH1:
            arm_->down();
            return polar_.runTo(200, -900);

        case StrategyPhase::SET_DIFFANGLE1:
            diffAngle = bodyAngleMeasurement_->getRelative10(); //現在の相対角度をセット
            return true;

        case StrategyPhase::ADJUST_ANGLE1:
            return polar_.bodyTurn(-900 - diffAngle, 20);

        case StrategyPhase::ARM_DOWN:
            return arm_->down();

        case StrategyPhase::ACCESS1:
            return polar_.runTo(50, 0);

        case StrategyPhase::PICKUP_PRIZE:
            return arm_->setDegree(60);

        case StrategyPhase::BACK1:
            polar_.back(true);
            polar_.setMaxPwm(20);
            return polar_.runTo(250, 1800);

        case StrategyPhase::SET_DIFFANGLE2:
            polar_.back(false);
            diffAngle = bodyAngleMeasurement_->getRelative10(); //現在の相対角度をセット
            //return polar_.bodyTurn(900, 20);
            //return polar_.bodyTurn(0 - bodyAngleMeasurement_->getRelative10(), 20);
            return true;

        case StrategyPhase::TURN2:
            return polar_.bodyTurn(0 - diffAngle, 20);

        case StrategyPhase::WAIT_TRAIN:
            if(!train_->atExit()){
                return true;
            }
            return false;

        case StrategyPhase::LEAVE_AREA:
            polar_.setMaxPwm(40);
            return polar_.runTo(520, 0);

        case StrategyPhase::APPROACH2:
            return polar_.runTo(100, -900);
        
        case StrategyPhase::SET_DIFFANGLE3:
            diffAngle = bodyAngleMeasurement_->getRelative10(); //現在の相対角度をセット
            return true;

        case StrategyPhase::ADJUST_ANGLE3:
            return polar_.bodyTurn(-900 - diffAngle, 20);
        
        case StrategyPhase::PUTDOWN_PRIZE:
            return arm_->setDegree(15);
        
        case StrategyPhase::BACK2:
            polar_.back(true);
            return polar_.runTo(80, 1800);

        case StrategyPhase::SET_DIFFANGLE4:
            polar_.back(false);
            diffAngle = bodyAngleMeasurement_->getRelative10(); //現在の相対角度をセット
            return true;

        case StrategyPhase::TURN3:
            return polar_.bodyTurn(0 - diffAngle, 20);

        case StrategyPhase::ARM_RESET:
            return arm_->normal();

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
