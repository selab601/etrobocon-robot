#ifndef CATCHING_H
#define CATCHING_H

#include "./PivotTurn.h"
#include "./Destination.h"
#include "./Avoidance.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "./CurveRunning.h"
#include "./LineTrace.h"
#include "../detection/ColorDetection.h"
#include "../device/ColorSensor.h"
#include <stdlib.h>

namespace drive
{
    class Catching{
        private:

            enum class State
            {
                INIT,
                TURN_RIGHT,
                TO_BLOCK,
                TURN,
                TO_LINE,
                FINISHED,
            };

            State state_ = State::INIT;


        public:
            Catching();

        private:

    };
}
#endif
