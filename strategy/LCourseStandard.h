#ifndef _LCOURSE_STANDARD_H
#define _LCOURSE_STANDARD_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include <vector>

namespace strategy{
    class LCourseStandard : public IStrategy{
        private:
            enum class Phase
            {
                LINETRACE1,
                LINETRACE2,
                LINETRACE3,
                LINETRACE4,
                LINETRACE5,
                LINETRACE6,
                CHANGEEDGE,
                CURVE1,
                CURVE2,
                CURVE3,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::LINETRACE1,
                Phase::CURVE1,
                Phase::LINETRACE2,
                Phase::CURVE2,
                Phase::LINETRACE3,
                Phase::CHANGEEDGE,
                Phase::LINETRACE4,
                Phase::CURVE3,
                Phase::LINETRACE5,
                Phase::CHANGEEDGE,
                Phase::LINETRACE6,
            };

            drive::LineTrace* linetrace_;
            measurement::DistanceMeasurement* distanceMeasurement_;

            bool strategySuccess_;
            bool hasExecutedPhase_;

        public:
            //コンストラクタ
            LCourseStandard();

            bool capture();
        private:
            bool executePhase(Phase phase);
            void startDistanceMeasurement(int distance);
            bool fixedDistanceLineTrace(int distance,int speed,drive::LineTraceEdge edge);
        };
}

#endif
