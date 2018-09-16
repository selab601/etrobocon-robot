
#ifndef _LCOURSE_STANDARD_H
#define _LCOURSE_STANDARD_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "drive/CurveRunning.h"
#include "measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
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
                CHANGEEDGE_L_R,
                CURVE1,
                CURVE2,
                CURVE3,
                CURVE4,
                CURVE5,
                STOP,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::LINETRACE1,
                Phase::CURVE1,
                Phase::LINETRACE2,
                Phase::CURVE2,
                Phase::LINETRACE3,
                Phase::CURVE3,
                Phase::LINETRACE4,
                Phase::CURVE4,
                Phase::STOP,
            
            };

            drive::LineTrace* linetrace_;
            drive::CurveRunning* curveRunning_;
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
