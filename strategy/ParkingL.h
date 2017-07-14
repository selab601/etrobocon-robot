#ifndef PARKING_L
#define PARKING_L

#include "IStrategy.h"
#include "../device/Arm.h"
#include "../drive/CurveRunning.h"
#include "../drive/PivotTurn.h"
#include "../drive/StraightRunning.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/TimeMeasurement.h"
#include <vector>

namespace strategy{
    class ParkingL : public IStrategy{
        private:
            enum class Phase{
                ADJUST,
                PIVOT_TURN1,
                APPROACH,
                PIVOT_TURN2,
                ENTRY,
                PIVOT_TURN3,
                WAIT,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::ADJUST,
                Phase::PIVOT_TURN1,
                Phase::APPROACH,
                Phase::PIVOT_TURN2,
                Phase::ENTRY,
                Phase::PIVOT_TURN3,
                Phase::WAIT,
            };

            //device::Arm* arm_;
            drive::CurveRunning* curveRunning_;
            drive::PivotTurn* pivotTurn_;
            drive::StraightRunning* straightRunning_;
            measurement::DistanceMeasurement* distanceMeasurement_;
            measurement::TimeMeasurement* timeMeasurement_;

            bool strategySuccess_;
        
        public:
            //ブロック並べ終了→ピボット→前進→ピボット→前進→ピボットで駐車
            //ピボットからカーブに変更するかもしれません
            ParkingL();

            bool capture();
        
        private:
            bool executePhase(Phase phase);
            
    };
}
#endif
