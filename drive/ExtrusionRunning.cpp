#include "ExtrusionRunning.h"

using namespace measurement;

namespace drive{

    //コンストラクタ
    ExtrusionRunning::ExtrusionRunning(){
        straightRunning_ = StraightRunning::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        runningState_ = INIT;
    }


    bool ExtrusionRunning::run(int speed , int distance){
        switch(runningState_){
        case INIT:
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            runningState_ = FORWARD;
            break;

        case FORWARD:
            if(specifiedDistanceRunning(speed)){
                distanceMeasurement_->startMeasurement();
                runningState_ = BACKWARD;
            }
            break;
        case BACKWARD:
            if(specifiedDistanceRunning(speed)){
                return true;
            }
            break;
        }
        return false;
    }

    bool ExtrusionRunning::specifiedDistanceRunning(int speed){
        straightRunning_->run(speed);
        if(distanceMeasurement_->getResult()){
            return true;
        }
        return false;
    }
}
