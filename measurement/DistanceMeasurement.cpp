#include "DistanceMeasurement.h"

namespace measurement {
    DistanceMeasurement::DistanceMeasurement() {
        base_ = 0;
        target_ = 0;
        selfPositionEstimation_ = SelfPositionEstimation::getInstance();
        isStart_ = false;
    }

    bool DistanceMeasurement::getResult() {
        if (getRelative() >= target_) {
            return true;
        }
        return false;
    }

    void DistanceMeasurement::start() {
        base_ = selfPositionEstimation_->getMigrationLength();
        isStart_ = true;
    }

    void DistanceMeasurement::start(int target){
        if(!isStart_){
            start();
            setTarget(target);
        }
    }

    void DistanceMeasurement::setTarget(int target) {
        if(target > 0){
            target_ = target;
        }
    }

    void DistanceMeasurement::reset(){
        base_ = 0;
        target_ = 0;
        isStart_ = false;
    }

    long DistanceMeasurement::getRemaining(){
        if(isStart_){
            return target_ + base_ - getRelative();
        }
        return -1;
    }

    long DistanceMeasurement::getRelative(){
        if(isStart_){
            return selfPositionEstimation_->getMigrationLength() - base_;
        }
        return -1;
    }
};
