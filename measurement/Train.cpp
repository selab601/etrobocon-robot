#include "Train.h"

namespace measurement
{

    Train* Train::instance_ = 0;

    Train* Train::getInstance(){
        if(instance_ == 0){
            instance_ = new Train();
        }
        return instance_;
    }

    Train::Train(){
        timeMeasurement_ = TimeMeasurement();
    }

    void Train::start(){
        timeMeasurement_.setBaseTime();
    }

    bool Train::atEntrance(){
        int32_t trainStart = OUT_CYCLE_TIME * 4 / 8;
        int32_t trainStart2 = OUT_CYCLE_TIME + IN_CYCLE_TIME * 2 / 8;
        return (getCycleTime() <= trainStart + TRAIN_LENGTH &&
            getCycleTime() >= trainStart) ||
            (getCycleTime() <= trainStart2 + TRAIN_LENGTH &&
             getCycleTime() >= trainStart2);
    }

    bool Train::atCenter(){
        int32_t trainStart = OUT_CYCLE_TIME * 6 / 8;
        return getCycleTime() <= trainStart + TRAIN_LENGTH &&
            getCycleTime() >= trainStart;
    }
    bool Train::atExit(){
        int32_t trainStart = 0;
        return getCycleTime() <= trainStart + TRAIN_LENGTH &&
            getCycleTime() >= trainStart;
    }

    int32_t Train::getCycleTime(){
        int32_t cycleTime = ADJUST_LENGTH + timeMeasurement_.getRelative();
        cycleTime = cycleTime % (IN_CYCLE_TIME + OUT_CYCLE_TIME);
        if (cycleTime <= OUT_CYCLE_TIME){
            isOut_ = true; // 最初は外側を走る
        }
        else{
            isOut_ = false; // 次に内側を走る
        }
        return cycleTime;
    }
}
