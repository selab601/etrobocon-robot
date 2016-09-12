#ifndef BACK_TO_LINE_STATE_H_
#define BACK_TO_LINE_STATE_H_

#include "IBlockColorGetterState.h"
#include "device/Arm.h"
#include "drive/StraightRunning.h"
#include "measurement/DistanceMeasurement.h"
#include <queue>

namespace drive {
    class BackToLineState : public IBlockColorGetterState {
    private:
        const static int ANGLE = 35;
        enum class BACK_STATE {
            INIT_TIME,
            INIT_DISTANCE,
            SET_ARM,
            BACK,
            FINISH
        };
        std::queue<BACK_STATE> states_;
        drive::StraightRunning* straight_;
        measurement::DistanceMeasurement* distanceMeasurement_;
        bool initialized_;
        int baseCount_;
    public:
        BackToLineState();
        bool isExecuted(colorset_t* result);
    };
}

#endif
