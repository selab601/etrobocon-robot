#include "BodyangleDetection.h"

namespace measurement {
  BodyangleDetection::BodyangleDetection() {
    baceAngle = 0;
    selfpositionestimation = SelfPositionEstimation::getInstance();
    display = device::Display::getInstance();
  }

  void BodyangleDetection::startDetection() {
    baceAngle = selfpositionestimation->getAngle();
  }

  int BodyangleDetection::getBodyangleDetection() {
    int currentAngle = selfpositionestimation->getAngle();
    return (currentAngle - baceAngle);
  }

}
