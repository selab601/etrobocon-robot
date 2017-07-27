#ifndef PARKING_L
#define PARKING_L

#include "IStrategy.h"
#include "../detection/LineDetection.h"
#include "../device/Arm.h"
#include "../drive/PivotTurn.h"
#include "../drive/LineTrace.h"
#include "../drive/StraightRunning.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/TimeMeasurement.h"
#include <vector>

namespace strategy{
    class ParkingL : public IStrategy{
        private:
            enum class Phase{
                ADJUST1,
                PIVOT_TURN1,
                APPROACH,
                PIVOT_TURN2,
                LINETRACE,
                PIVOT_TURN3,
                ENTRY,
                PIVOT_TURN4,
                FINISH,
                WAIT,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::ADJUST1,
                Phase::PIVOT_TURN1,
                Phase::APPROACH,
                Phase::PIVOT_TURN2,
                Phase::LINETRACE,
                Phase::PIVOT_TURN3,
                Phase::ENTRY,
                Phase::PIVOT_TURN4,
                Phase::FINISH,
                Phase::WAIT,
            };

            detection::LineDetection* lineDetection_;
            device::Arm* arm_;
            drive::PivotTurn* pivotTurn_;
            drive::LineTrace* lineTrace_;
            drive::StraightRunning* straightRunning_;
            measurement::DistanceMeasurement* distanceMeasurement_;
            measurement::TimeMeasurement* timeMeasurement_;

            bool strategySuccess_;
        
        public:
            //ブロック並べ終了→ピボット→前進→ピボット→前進→ピボットで駐車
            //超信地旋回から信地旋回に変更するかもしれません
            ParkingL();

            bool capture();
        
        private:
            bool executePhase(Phase phase);
            
    };
}
#endif
