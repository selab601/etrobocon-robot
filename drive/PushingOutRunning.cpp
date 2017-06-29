#include "PushingOutRunning.h"

using namespace measurement;

namespace drive{

    PushingOutRunning::PushingOutRunning(){
        straightRunning_ = new StraightRunning();
        distanceMeasurement_ = new DistanceMeasurement();
        timeMeasurement_ = new TimeMeasurement();
        runningState_ = RunningState::INIT;
    }


    bool PushingOutRunning::run(int speed, int distance){
        switch(runningState_){
        //初期状態...目標距離セット
        case RunningState::INIT:
                distanceMeasurement_->setTarget(distance);
                distanceMeasurement_->start();
                runningState_ = RunningState::FORWARD;
            break;

        //前進
        case RunningState::FORWARD:
            straightRunning_->run(speed);
            if(distanceMeasurement_->getResult()){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(200);
                runningState_ = RunningState::STOP;
            }
            break;

        //一時停止...いきなりバックすると車体がぶれるため
        case RunningState::STOP:
            straightRunning_->run(0);
            if(timeMeasurement_->getResult()){
                distanceMeasurement_->setTarget(distance);
                distanceMeasurement_->start();
                runningState_ = RunningState::BACKWARD;
            }
            break;

        //後退
        case RunningState::BACKWARD:
            straightRunning_->run(-speed);
            if(distanceMeasurement_->getResult()){
                runningState_ = RunningState::INIT;//初期状態に戻しておく
                return true;
            }
            break;
        }
        return false;
    }
}
