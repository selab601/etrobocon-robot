#include "DistanceDetection.h"

namespace measurement {
  DistanceDetection::DistanceDetection() {
    baseDistance = 0;
    targetDistance = 0;
    selfpositionestimation = SelfPositionEstimation::getInstance();
  }

  bool DistanceDetection::getResult() {
    long currentDistance = selfpositionestimation->getMigrationLength();
    long def = currentDistance - baseDistance;

    if (def >= targetDistance) {
      return true;

    }

    return false;
  }

  void DistanceDetection::startDetection() {
    baseDistance = selfpositionestimation->getMigrationLength();
  }

  void DistanceDetection::setTargetDistance(int distance) {
    targetDistance = distance;
  }
};
