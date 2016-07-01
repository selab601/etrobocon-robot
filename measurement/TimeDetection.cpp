#include "TimeDetection.h"

namespace detection{
  TimeDetection::TimeDetection(){
    clock           = Clock();
    base_time_      = 0;
    target_time_    = 0;
  }

  bool TimeDetection::isDetected(){
    uint32_t now_time_ = clock.now();
    uint32_t def = now_time_ - base_time_;

    if(def >= target_time_){
      return true;
    }return false;
  }

  void TimeDetection::setBaseTime (){
    base_time_ = clock.now();
  }

  void TimeDetection::setTargetTime(uint32_t target_time){
    target_time_ = target_time;
  }
};
