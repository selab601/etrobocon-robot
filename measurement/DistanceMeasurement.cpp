#include "DistanceMeasurement.h"

namespace measurement {
  DistanceMeasurement::DistanceMeasurement() {
    baseDistance = 0;
    targetDistance = 0;
    selfpositionestimation = SelfPositionEstimation::getInstance();
  }

  bool DistanceMeasurement::getResult() {
    long currentDistance = selfpositionestimation->getMigrationLength();
    long def = currentDistance - baseDistance;

    if (def >= targetDistance) {
      return true;

    }

    return false;
  }

  void DistanceMeasurement::startMeasurement() {
    baseDistance = selfpositionestimation->getMigrationLength();
  }

  void DistanceMeasurement::setTargetDistance(int distance) {
    targetDistance = distance;
  }
};
