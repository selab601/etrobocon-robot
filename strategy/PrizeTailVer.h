#ifndef PRIZE_TAIL_VER_H
#define PRIZE_TAIL_VER_H

#include "IStrategy.h"
#include "../drive/StraightRunning.h"
#include "../drive/PivotTurn.h"
#include "../drive/LineTrace.h"
#include "../drive/CurveRunning.h"
#include "../detection/LineDetection.h"
#include "../detection/ObjectDetection.h"
#include "../measurement/TimeMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "../device/Motors.h"
#include <vector>



namespace strategy{
    class PrizeTailVer : public IStrategy{
    private:
        enum class Phase{
            LINE_TRACE,
            DOWN_ARM,
            PUT_IN_LOWER,
            LIFT,
            DOWN,
            BACK,
            BACK2,
            TURN,
            CURVE,
            STRAIGHT_1_CM,
            TAIL_MOVE,
            ZERO
        };

        std::vector<Phase> phaseProcedure_{
            Phase::LINE_TRACE,
            Phase::DOWN_ARM,
            Phase::PUT_IN_LOWER,
            Phase::LIFT,
            Phase::DOWN,
            Phase::BACK,
            Phase::TURN,
            Phase::CURVE,
            Phase::BACK2,
            Phase::TURN,
            Phase::STRAIGHT_1_CM,
            Phase::TAIL_MOVE,
            Phase::ZERO
        };

        drive::PivotTurn* pivotTurn_;
        drive::CurveRunning* curveRunning_;
        drive::StraightRunning* straightRunning_;
        drive::LineTrace* lineTrace_;

        detection::ObjectDetection* objectDetection_;
        detection::LineDetection* lineDetection_;

        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;

        device::Motors* motors_;

        bool strategySuccess_;
        bool hasExecutedPhase_;
    public:
        //コンストラクタ
        PrizeTailVer();

        /**
         * @brief 懸賞運びを攻略する
         * @return 攻略完了:true,攻略中:false
         */
        bool capture();

    private:
        bool executePhase(Phase phase);
        void startTimeMeasurement(int time);
        void startDistanceMeasurement(int distance);

    };
}

#endif
