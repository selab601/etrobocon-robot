#include "TimeMeasurement.h"

namespace measurement{
    TimeMeasurement::TimeMeasurement(){
        clock           = ev3api::Clock();
        baseTime_      = 0;
        targetTime_    = 0;
    }

    bool TimeMeasurement::getResult(){
        uint32_t nowTime_ = clock.now();
        uint32_t def = nowTime_ - baseTime_;

        if(def >= targetTime_){
            return true;
        }return false;
    }

    void TimeMeasurement::setBaseTime (){
        baseTime_ = clock.now();
    }

    void TimeMeasurement::setTargetTime(uint32_t targetTime){
        targetTime_ = targetTime;
    }
};
