#ifndef PARKING_R
#define PARKING_R

#include "IStrategy.h"
#include "../drive/LineTrace.h"
#include "../drive/PivotTurn.h"
#include "../drive/StraightRunning.h"
#include "../detection/RightAngledDetection.h"
#include "../detection/ColorDetection.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/TimeMeasurement.h"
#include <vector>

namespace strategy{
    class ParkingR : public IStrategy{
        private:
            enum class Phase{
                INIT,
                STRAIGHT1,
                ADJUST1,
                TURN_LEFT,
                STRAIGHT2,
                WAIT,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::INIT,
                Phase::STRAIGHT1,
                Phase::ADJUST1,
                Phase::TURN_LEFT,
                Phase::STRAIGHT2,
                Phase::WAIT,
            };

            drive::LineTrace* lineTrace_;
            drive::PivotTurn* pivotTurn_;
            drive::StraightRunning* straightRunning_;
            detection::RightAngledDetection* rightAngledDetection_;
            device::ColorSensor* colorSensor_;
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
