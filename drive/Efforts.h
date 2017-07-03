#ifndef EFFORTS_H_
#define EFFORTS_H_

#include "../drive/LineTrace.h"
#include "../drive/PivotTurn.h"
#include "../drive/CurveRunning.h"
#include "../drive/PushingOutRunning.h"
#include "../drive/ForcingOutRunning.h"
#include "../drive/BlockColorGetter.h"
#include "../detection/LineDetection.h"
#include "../detection/ColorDetection.h"
#include "../detection/RightAngledDetection.h"
#include <vector>

namespace drive{
    class Efforts{
    private:
        enum class Phase{
            INIT,
            LINETRACE_TO_DAIZA,
            BLOCK_COLOR_GET,
            KIMARITE,
            PIVORT_TURN,
            TURN_TO_LINE,
            LINETRACE_RIGHT_ANGLED,
        };

        std::vector<Phase> phaseProcedure_{
            Phase::INIT,
            Phase::LINETRACE_TO_DAIZA,
            Phase::BLOCK_COLOR_GET,
            Phase::KIMARITE,
            Phase::PIVORT_TURN,
            Phase::TURN_TO_LINE,
            Phase::LINETRACE_RIGHT_ANGLED
        };

        drive::LineTrace* lineTrace_;
        drive::PivotTurn* pivotTurn_;
        drive::CurveRunning* curveRunning_;
        drive::BlockColorGetter* blockColorGetter_;
        drive::PushingOutRunning* pushingOutRunning_;
        drive::ForcingOutRunning* forcingOutRunning_;
        detection::LineDetection* lineDetection_;
        detection::ColorDetection* colorDetection_;
        detection::RightAngledDetection* rightAngledDetection_;

        bool isSuccess_;

        unsigned int procedureNumber_ ;

        drive::colorset_t* result_;

        int positionNumber_;

        drive::LineTraceEdge startEdge_;
        drive::LineTraceEdge endEdge_;
        bool isRightCurve_;
        bool isRightForcingOut_;
        int turnAngle_;

    public:
        //コンストラクタ
        Efforts();


        bool run(int positionNumber);

    private:

        bool executePhase(Phase phase);
    };
}

#endif
