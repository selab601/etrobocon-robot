#include "DistanceMeasurement.h"

namespace measurement {
  DistanceMeasurement::DistanceMeasurement() {
    baseDistance_ = 0;
    targetDistance_ = 0;
    selfPositionEstimation_ = SelfPositionEstimation::getInstance();
    isStartMeasurement_ = false;
  }

  bool DistanceMeasurement::getResult() {
    long currentDistance = selfPositionEstimation_->getMigrationLength();
    long def = currentDistance - baseDistance_;

    if (def >= targetDistance_) {
      return true;

    }

    return false;
  }

  void DistanceMeasurement::startMeasurement() {
    baseDistance_ = selfPositionEstimation_->getMigrationLength();
    isStartMeasurement_ = true;
  }

  void DistanceMeasurement::startMeasurement(int distance){
    if(!isStartMeasurement_){
        startMeasurement();
        setTargetDistance(distance);
    }
  }

  void DistanceMeasurement::setTargetDistance(int distance) {
    targetDistance_ = distance;
  }

  void DistanceMeasurement::reset(){
    baseDistance_ = 0;
    targetDistance_ = 0;
    isStartMeasurement_ = false;
  }

  long getRemainingDistance(){
    if(isStartMeasurement_){
        long currentDistance = selfPositionEstimation_->getMigrationLength();
        return currentDistance - baseDistance_;
    }
    return -1;
  }
};
