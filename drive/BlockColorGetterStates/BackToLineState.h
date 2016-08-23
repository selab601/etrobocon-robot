#ifndef BACK_TO_LINE_STATE_H_
#define BACK_TO_LINE_STATE_H_

#include "IBlockColorGetterState.h"
#include "device/Motors.h"
#include "drive/StraightRunning.h"
#include "measurement/CountMeasurement.h"
#include "measurement/DistanceMeasurement.h"
#include "measurement/TimeMeasurement.h"
#include <queue>

namespace drive {
    class BackToLineState : public IBlockColorGetterState {
    private:
        const static int ANGLE = 35;
        enum class BACK_STATE {
            INIT,
            DOWN_ARM,
            SET_ARM,
            BACK,
            FINISH
        };
        std::queue<BACK_STATE> states_;
        device::Motors* motors_;
        drive::StraightRunning* straight_;
        measurement::CountMeasurement* countMeasure_;
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;
        bool initialized_;
        int baseCount_;
    public:
        BackToLineState();
        bool isExecuted(colorset_t* result);
    };
}

#endif
