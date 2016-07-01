#include "../device/display.h"
device::Display* disp = device::Display::getInstance();

#include "CountDetection.h"

using namespace device;

namespace measurement{
  CountDetection::CountDetection(MotorKind KIND, int32_t target_count, int32_t base_count){
    motor         = Motors::getInstance();
    target_count_ = target_count;
    KIND_         = KIND;
    this->setBaseCount(base_count);
  }

  bool CountDetection::isDetected(){
    int32_t now_count = -1;
    switch (KIND_) {
    case MotorKind::RIGHT:
      now_count = motor->getCount(MOTOR_RIGHT);
      break;
    case MotorKind::LEFT:
      now_count = motor->getCount(MOTOR_LEFT);
    }
    disp->updateDisplay("now",now_count,10);
    disp->updateDisplay("base",base_count_,11);
    disp->updateDisplay("tar",target_count_,12);
    if (target_count_+base_count_ == now_count){ return true; }
    return false;
  }

  void CountDetection::setBaseCount(int32_t base_count){
    if (base_count == -1){
      switch (KIND_) {
      case MotorKind::RIGHT:
	base_count_ = motor->getCount(MOTOR_RIGHT);
	break;
      case MotorKind::LEFT:
	base_count_ = motor->getCount(MOTOR_LEFT);
      }
    } else {
      base_count_ = base_count;
    }
  }

  void CountDetection::setTargetCount(int32_t target_count){
    target_count_ = target_count;
  }

  void CountDetection::setCountConfig(int32_t target_count, int32_t base_count){
    this->setBaseCount(base_count);
    this->setTargetCount(target_count);
  }

  void CountDetection::setTargetMotor(MotorKind KIND){
    KIND_ = KIND;
  }

  CountDetection::PlusOrMinus CountDetection::comparedWithTargetCount(){
    int32_t now_count = -1;
    switch (KIND_) {
    case MotorKind::RIGHT:
      now_count = motor->getCount(MOTOR_RIGHT);
      break;
    case MotorKind::LEFT:
      now_count = motor->getCount(MOTOR_LEFT);
    }

    if (target_count_+base_count_ > now_count){
      return PlusOrMinus::PLUS;
    }
    return PlusOrMinus::MINUS;
  }
};
