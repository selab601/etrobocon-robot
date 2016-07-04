#include "TimeDetection.h"

namespace measurement{
  TimeDetection::TimeDetection(){
    clock           = ev3api::Clock();
    baseTime_      = 0;
    targetTime_    = 0;
  }

  bool TimeDetection::isDetected(){
    uint32_t nowTime_ = clock.now();
    uint32_t def = nowTime_ - baseTime_;

    if(def >= targetTime_){
      return true;
    }return false;
  }

  void TimeDetection::setBaseTime (){
    baseTime_ = clock.now();
  }

  void TimeDetection::setTargetTime(uint32_t targetTime){
    targetTime_ = targetTime;
  }
};
