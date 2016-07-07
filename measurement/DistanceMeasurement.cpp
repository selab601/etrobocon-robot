#include "DistanceMeasurement.h"

namespace measurement {
  DistanceMeasurement::DistanceMeasurement() {
    baseDistance_ = 0;
    targetDistance_ = 0;
    selfpositionestimation_ = SelfPositionEstimation::getInstance();
  }

  bool DistanceMeasurement::getResult() {
    long currentDistance = selfpositionestimation_->getMigrationLength();
    long def = currentDistance - baseDistance_;

    if (def >= targetDistance_) {
      return true;

    }

    return false;
  }

  void DistanceMeasurement::startMeasurement() {
    baseDistance_ = selfpositionestimation_->getMigrationLength();
  }

  void DistanceMeasurement::setTargetDistance(int distance) {
    targetDistance_ = distance;
  }
};
