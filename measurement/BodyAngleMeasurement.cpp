#include "BodyAngleMeasurement.h"

namespace measurement {


  BodyAngleMeasurement::BodyAngleMeasurement() {
    baseAngle_ = 0;
    selfPositionEstimation_ = SelfPositionEstimation::getInstance();
  }

  void BodyAngleMeasurement::setBaseAngle() {
    baseAngle_ = selfPositionEstimation_->getAngle();
  }

  int BodyAngleMeasurement::getResult() {
    int currentAngle = selfPositionEstimation_->getAngle();
    return (currentAngle - baseAngle_);
  }

}
