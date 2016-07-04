#include "CountDetection.h"

namespace measurement{


  CountDetection::CountDetection(MotorKind KIND, int32_t targetCount, int32_t baseCount){
    motor         = device::Motors::getInstance();
    targetCount_ = targetCount;
    KIND_         = KIND;
    this->setBaseCount(baseCount);
  }

  bool CountDetection::getResult(){
    int32_t nowCount = -1;
    switch (KIND_) {
    case MotorKind::RIGHT:
      nowCount = motor->getCount(device::MOTOR_RIGHT);
      break;
    case MotorKind::LEFT:
      nowCount = motor->getCount(device::MOTOR_LEFT);
    }
    if (targetCount_+baseCount_ == nowCount){ return true; }
    return false;
  }

  void CountDetection::setBaseCount(int32_t baseCount){
    if (baseCount == -1){
      switch (KIND_) {
      case MotorKind::RIGHT:
	baseCount_ = motor->getCount(device::MOTOR_RIGHT);
	break;
      case MotorKind::LEFT:
	baseCount_ = motor->getCount(device::MOTOR_LEFT);
      }
    } else {
      baseCount_ = baseCount;
    }
  }

  void CountDetection::setTargetCount(int32_t targetCount){
    targetCount_ = targetCount;
  }

  void CountDetection::setCountConfig(int32_t targetCount, int32_t baseCount){
    this->setBaseCount(baseCount);
    this->setTargetCount(targetCount);
  }

  void CountDetection::setTargetMotor(MotorKind KIND){
    KIND_ = KIND;
  }

  measurement::PlusOrMinus CountDetection::comparedWithTargetCount(){
    int32_t nowCount = -1;
    switch (KIND_) {
    case MotorKind::RIGHT:
      nowCount = motor->getCount(device::MOTOR_RIGHT);
      break;
    case MotorKind::LEFT:
      nowCount = motor->getCount(device::MOTOR_LEFT);
    }

    if (targetCount_+baseCount_ > nowCount){
      return PlusOrMinus::PLUS;
    }
    return PlusOrMinus::MINUS;
  }
};
