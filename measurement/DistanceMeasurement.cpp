#include "DistanceMeasurement.h"

namespace measurement {
  DistanceMeasurement::DistanceMeasurement() {
    baseDistance_ = 0;
    targetDistance_ = 0;
    selfPositionEstimation_ = SelfPositionEstimation::getInstance();
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
  }

  void DistanceMeasurement::setTargetDistance(int distance) {
    targetDistance_ = distance;
  }
};
