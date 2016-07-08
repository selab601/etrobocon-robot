#include "ExtrusionRunning.h"

using namespace measurement;

namespace drive{

    //コンストラクタ
    ExtrusionRunning::ExtrusionRunning(){
        straightRunning_ = new StraightRunning();
        distanceMeasurement_ = new DistanceMeasurement();
        timeMeasurement_ = new TimeMeasurement();
        runningState_ = INIT;
    }


    bool ExtrusionRunning::run(int speed, int distance){
        switch(runningState_){
        //初期状態...目標距離のセットをする
        case INIT:
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            runningState_ = FORWARD;
            break;

        //前進
        case FORWARD:
            straightRunning_->run(speed);
            if(distanceMeasurement_->getResult()){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(500);
                runningState_ = STOP;
            }
            break;

        //一時停止...いきなりバックすると車体がぶれるため
        case STOP:
            straightRunning_->run(0);
            if(timeMeasurement_->getResult()){
                distanceMeasurement_->setTargetDistance(distance);
                distanceMeasurement_->startMeasurement();
                runningState_ = BACKWARD;
            }
            break;

        //後退
        case BACKWARD:
            straightRunning_->run(-speed);
            if(distanceMeasurement_->getResult()){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(100);
                runningState_ = END;
            }
            break;

        case END:
            straightRunning_->run(0);
            if(timeMeasurement_->getResult()){
                runningState_ = INIT;//初期状態に戻しておく
                return true;
            }
            break;
        }
        return false;
    }
}
