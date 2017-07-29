#ifndef PARKING_R
#define PARKING_R

#include "IStrategy.h"
#include "../drive/LineTrace.h"
#include "../drive/PivotTurn.h"
#include "../drive/StraightRunning.h"
#include "../detection/RightAngledDetection.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/TimeMeasurement.h"
#include <vector>

namespace strategy{
    class ParkingR : public IStrategy{
        private:
            enum class Phase{
                LINE_TRACE1,
                ADJUST1,
                TURN_RIGHT,
                LINE_TRACE2,
                WAIT,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::LINE_TRACE1,
                Phase::ADJUST1,
                Phase::TURN_RIGHT,
                Phase::LINE_TRACE2,
                Phase::WAIT,
            };

            drive::LineTrace* lineTrace_;
            drive::PivotTurn* pivotTurn_;
            drive::StraightRunning* straightRunning_;
            detection::RightAngledDetection* rightAngledDetection_;
            measurement::DistanceMeasurement* distanceMeasurement_;
            measurement::TimeMeasurement* timeMeasurement_;

            bool strategySuccess_;


        
        public:
            ParkingR();

            bool capture();
        
        private:
            bool executePhase(Phase phase);
            
    };
}
#endif
