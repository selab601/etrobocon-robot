#ifndef PARKING_L
#define PARKING_L

#include "IStrategy.h"
#include "../detection/LineDetection.h"
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
                LINETRACE,
                TURN_LEFT_60,
                STRAIGHT_36,
                TURN_RIGHT_90,
                FINISH,
                WAIT,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::LINETRACE,
                Phase::TURN_LEFT_60,
                Phase::STRAIGHT_36,
                Phase::TURN_RIGHT_90,
                Phase::FINISH,
                Phase::WAIT,
            };

            detection::LineDetection* lineDetection_;
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
