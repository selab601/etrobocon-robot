#include "BodyAngleMeasurement.h"

namespace measurement {


  BodyAngleMeasurement::BodyAngleMeasurement() {
    baseAngle_ = 0;
    selfpositionestimation_ = SelfPositionEstimation::getInstance();
  }

  void BodyAngleMeasurement::setBaseAngle() {
    baseAngle_ = selfpositionestimation_->getAngle();
  }

  int BodyAngleMeasurement::getResult() {
    int currentAngle = selfpositionestimation_->getAngle();
    return (currentAngle - baseAngle_);
  }

}
