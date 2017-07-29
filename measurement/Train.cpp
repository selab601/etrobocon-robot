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

    void Train::setEntrance(){
        if (OUT_CYCLE_TIME * 1/4 <= getCycleTime() &&
                OUT_CYCLE_TIME + IN_CYCLE_TIME * 3/16 > getCycleTime()){
            timeMeasurement_.setBaseTime(-OUT_CYCLE_TIME * 3/4);
        }
        else{
            timeMeasurement_.setBaseTime(-OUT_CYCLE_TIME *3/4 - IN_CYCLE_TIME);
        }
    }

    void Train::setCenter(){
        timeMeasurement_.setBaseTime(-OUT_CYCLE_TIME * 3/4 - IN_CYCLE_TIME * 1/2);
    }


    bool Train::atEntrance(){
        int32_t trainStart = OUT_CYCLE_TIME * 4/8 - getPassTime()/2;
        int32_t trainStart2 = OUT_CYCLE_TIME * 4/8 + IN_CYCLE_TIME - getPassTime()/2;
        int32_t trainEnd = trainStart + getPassTime();
        int32_t trainEnd2 = trainStart2 + getPassTime();
        return (getCycleTime() <= trainEnd &&
            getCycleTime() >= trainStart) ||
            (getCycleTime() <= trainEnd2 &&
             getCycleTime() >= trainStart2);
    }

    bool Train::atCenter(){
        int32_t trainStart = OUT_CYCLE_TIME * 4/8 + IN_CYCLE_TIME * 1/2 - getPassTime()/2;
        int32_t trainEnd = trainStart + getPassTime();
        return getCycleTime() <= trainEnd &&
            getCycleTime() >= trainStart;
    }
    bool Train::atExit(){
        int32_t trainStart = -1 - getPassTime()/2;
        int32_t trainEnd = trainStart + getPassTime();
        trainStart = getCycleTime(trainStart);
        // 1周の境界をまたぐため and でなく or
        return  getCycleTime() <= trainEnd ||
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

    int32_t Train::getCycleTime(int32_t time){
        // 0より小さい時、mod演算では出せない
        while (time < 0){
            time += (IN_CYCLE_TIME + OUT_CYCLE_TIME);
        }
        return time % (IN_CYCLE_TIME + OUT_CYCLE_TIME);
    }

    int Train::getPassTime(){
        int passTime = TRAIN_LENGTH;
        passTime += getCycleCount10() * EXTEND_PER_CYCLE / 10;
        return passTime;
    }

    int Train::getCycleCount10(){
        return 10 * timeMeasurement_.getRelative() / (IN_CYCLE_TIME + OUT_CYCLE_TIME);
    }
}
