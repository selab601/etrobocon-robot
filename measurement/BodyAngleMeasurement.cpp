#include "BodyAngleMeasurement.h"

namespace measurement {


  BodyAngleMeasurement::BodyAngleMeasurement() {
    baceAngle_ = 0;
    selfpositionestimation = SelfPositionEstimation::getInstance();
  }

  void BodyAngleMeasurement::setBaceAngle() {
    baceAngle_ = selfpositionestimation->getAngle();
  }

  int BodyAngleMeasurement::getResult() {
    int currentAngle = selfpositionestimation->getAngle();
    return (currentAngle - baceAngle_);
  }

}
