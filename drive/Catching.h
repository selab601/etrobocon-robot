#ifndef CATCHING_H
#define CATCHING_H

#include "../measurement/BodyAngleMeasurement.h"
#include "./CurveRunning.h"
#include "./LineTrace.h"
#include "../detection/ColorDetection.h"

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
