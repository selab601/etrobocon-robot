#include "ExtrusionRunning.h"

using namespace measurement;

namespace drive{

    //コンストラクタ
    ExtrusionRunning::ExtrusionRunning(){
        straightRunning_ = StraightRunning::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        timeMeasurement_ = new TimeMeasurement();
        runningState_ = INIT;
    }


    bool ExtrusionRunning::run(int speed , int distance){
        switch(runningState_){
        //初期化...目標距離のセットをする
        case INIT:
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            runningState_ = FORWARD;
            break;

        //前進
        case FORWARD:
            if(specifiedDistanceRunning(speed)){
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

        //後退
        case BACKWARD:
            if(specifiedDistanceRunning(-speed)){
                return true;
            }
            break;
        }
        return false;
    }

    //指定距離走行
    bool ExtrusionRunning::specifiedDistanceRunning(int speed){
        straightRunning_->run(speed);
        if(distanceMeasurement_->getResult()){
            return true;
        }
        return false;
    }
}
