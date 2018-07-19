#include "TimeMeasurement.h"

namespace measurement{
    TimeMeasurement::TimeMeasurement(){
        clock           = ev3api::Clock();
        baseTime_      = 0;
        targetTime_    = 0;
        isStart_ = false;
    }

    void TimeMeasurement::start(uint32_t targetTime){
        if(!isStart_){
            setBaseTime();
            setTargetTime(targetTime);
            isStart_ = true;
        }
    }

    bool TimeMeasurement::getResult(){
        if(getRelative() >= targetTime_){
            return true;
        }return false;
    }

    uint32_t TimeMeasurement::getRelative(){
        uint32_t nowTime = clock.now();
        uint32_t def = nowTime - baseTime_;
        return def;
    }

    uint32_t TimeMeasurement::getRemaining(){
        return targetTime_ - getRelative();
    }

    void TimeMeasurement::setBaseTime (){
        baseTime_ = clock.now();
    }
    void TimeMeasurement::setBaseTime(uint32_t relativeMs){
        baseTime_ -= clock.now() + relativeMs;
    }

    void TimeMeasurement::setTargetTime(uint32_t targetTime){
        targetTime_ = targetTime;
    }

    void TimeMeasurement::reset(){
        baseTime_ = 0;
        targetTime_ = 0;
        isStart_ = false;
    }
};
