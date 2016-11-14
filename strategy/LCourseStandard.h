#ifndef _LCOURSE_STANDARD_H
#define _LCOURSE_STANDARD_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include <vector>

#define LCOURSE_SHORTCUT_LENGTH 25     // ショートカットする長さ[mm]

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
                LINE_IGNORE,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::LINETRACE1,
                Phase::CURVE1,
                Phase::LINETRACE2,
                Phase::CURVE2,
                Phase::LINETRACE3,
                //Phase::CHANGEEDGE,
                Phase::LINE_IGNORE,
                //Phase::LINETRACE4,
                Phase::CURVE3,
                //Phase::LINETRACE5,
                //Phase::CHANGEEDGE,
                Phase::LINETRACE6,
            };

            drive::LineTrace* linetrace_;
            measurement::DistanceMeasurement* distanceMeasurement_;
            measurement::BodyAngleMeasurement bodyAngleMeasurement_;

            bool strategySuccess_;
            bool hasExecutedPhase_;
            bool isLineTraceReset_;

        public:
            //コンストラクタ
            LCourseStandard();

            bool capture();
        private:
            bool executePhase(Phase phase);
            void startDistanceMeasurement(int distance);
            void startAngleMeasurement();
            /**
             * @brief 一定距離ライントレース
             *
             * @param distance 走行距離
             * @param speed 走行スピード
             * @param edge ライントレースのエッジ
             * @return 一定距離走行:true,走行中:false
             */
            bool fixedDistanceLineTrace(int distance,int speed,drive::LineTraceEdge edge);
            /**
             * @brief 一定距離カーブライントレース
             *
             * @param distance 走行距離
             * @param deltaRad 角度？
             * @return 一定距離走行:true,走行中:false
             */
            bool fixedDistanceCurveLineTrace(int distance,int deltaRad);

            /**
             * @brief LineTrace::reset()を一度だけ実行する
             */
            void lineTraceReset();
        };
}

#endif
