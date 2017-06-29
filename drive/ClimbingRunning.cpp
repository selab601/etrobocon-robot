#include "ClimbingRunning.h"

using namespace measurement;
using namespace device;

namespace drive{

    //コンストラクタ
    ClimbingRunning::ClimbingRunning(){
        arm_ = Arm::getInstance();
        straightRunning_ = new StraightRunning();
        distanceMeasurement_ = new DistanceMeasurement();
        runningState_ = RunningState::ARM_UP;
    }


    bool ClimbingRunning::run(int speed , int distance){
        switch(runningState_){

        //アームを上げる
        case RunningState::ARM_UP:
            if(arm_->up()){
                distanceMeasurement_->setTarget(distance);
                distanceMeasurement_->start();
                runningState_ = RunningState::CLIMB;
            }
            break;

        //登壇する
        case RunningState::CLIMB:
            straightRunning_->run(speed);
            if(distanceMeasurement_->getResult()){
                runningState_ = RunningState::ARM_DOWN;
            }
            break;

        //アームを下げる
        case RunningState::ARM_DOWN:
            straightRunning_->run(0);
            if(arm_->normal()){
                runningState_ = RunningState::END;
            }
            break;

        //終了
        case RunningState::END:
            return true;
            break;
        }
        return false;
    }
}
