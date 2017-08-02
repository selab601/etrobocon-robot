#ifndef _LCOURSE_SHORTCUT_H
#define _LCOURSE_SHORTCUT_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include "measurement/BodyAngleMeasurement.h"
#include "drive/PolarRunning.h"
#include "device/ColorSensor.h"
#include <vector>

namespace strategy{
    class LCourseShortcut : public IStrategy{
        private:
            enum class Phase
            {
                INIT,
                LINETRACE_STRAIGHT,     // 最初の直線
                LINETRACE_RIGHT,        // 左カーブ
                LINETRACE_LEFT,         // ゆるい右カーブ(直線っぽいとこまで)

                LINETRACE_TO_GATE2,     // GATE2の辺りまで
                POLAR_TO_GOAL_FROM_GATE2,   // ゴールまでライン無視

                TO_LINE,                // ラインを見つける
                LINE_RETURN,            // ライン復帰
                LINETRACE_TO_BLOCKAREA, // ブロック並べの入り口まで

                STOP,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::INIT,

                Phase::LINETRACE_STRAIGHT,
                Phase::LINETRACE_RIGHT,
                Phase::LINETRACE_LEFT,  // まっすぐになってるとこまで

                Phase::LINETRACE_TO_GATE2,
                Phase::POLAR_TO_GOAL_FROM_GATE2,

                Phase::TO_LINE, // ライン復帰
                Phase::LINE_RETURN,
                Phase::LINETRACE_TO_BLOCKAREA,

            };

            drive::LineTrace* linetrace_;
            drive::PolarRunning polar_;
            measurement::DistanceMeasurement* distanceMeasurement_;
            measurement::BodyAngleMeasurement bodyAngleMeasurement_;

            bool strategySuccess_;
            bool hasExecutedPhase_;
            bool isLineTraceReset_;

        public:
            //コンストラクタ
            LCourseShortcut();

            bool capture();
        private:
            bool executePhase(Phase phase);
            void startDistanceMeasurement(int distance);
            void startAngleMeasurement();

            bool isBlack();
        };
}

#endif
