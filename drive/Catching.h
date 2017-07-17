#ifndef CATCHING_H
#define CATCHING_H

#include "../measurement/BodyAngleMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "./CurveRunning.h"
#include "./LineTrace.h"
#include "./StraightRunning.h"
#include "../detection/ColorDetection.h"

namespace drive
{
    class Catching{
        private:

            enum class Phase
            {
                START_LINE_TRACE,
                CURVE,
                STRAIGHT,
                END_LINE_TRACE
            };

            Phase phase_ = Phase::START_LINE_TRACE;

            measurement::BodyAngleMeasurement* bodyAngleMeasurement_;
            measurement::DistanceMeasurement* distanceMeasurement_;

            detection::ColorDetection* colorDetection_;

            CurveRunning* curveRunning_;
            LineTrace* lineTrace_;
            StraightRunning* straightRunning_;


        public:
            Catching();

            bool run(int digree);

        private:

    };
}
#endif
