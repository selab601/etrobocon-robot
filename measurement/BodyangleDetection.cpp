#include "BodyAngleDetection.h"

namespace measurement {


  BodyAngleDetection::BodyAngleDetection() {
    baceAngle_ = 0;
    selfpositionestimation = SelfPositionEstimation::getInstance();
  }

  void BodyAngleDetection::setBaceAngle() {
    baceAngle_ = selfpositionestimation->getAngle();
  }

  int BodyAngleDetection::getBodyAngle() {
    int currentAngle = selfpositionestimation->getAngle();
    return (currentAngle - baceAngle_);
  }

}
