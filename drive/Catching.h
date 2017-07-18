#ifndef CATCHING_H
#define CATCHING_H

#include "../measurement/BodyAngleMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "./CurveRunning.h"
#include "./LineTrace.h"
#include "./StraightRunning.h"
#include "../detection/ColorDetection.h"
#include <stdlib.h>

#define CATCHING_PWM 20
#define LINETRACE_PWM 20

namespace drive
{
    class Catching{
        private:
            //走行状態
            enum class Phase
            {
                START_LINE_TRACE,//色検知までライントレース
                STRAIGHT,        //直進走行(角度が大きい場合)
                CURVE,           //台座の上でカーブ走行
                END_LINE_TRACE   //カーブ後のライントレース
            };

            Phase phase_ = Phase::START_LINE_TRACE;

            measurement::BodyAngleMeasurement* bodyAngleMeasurement_;
            measurement::DistanceMeasurement* distanceMeasurement_;

            detection::ColorDetection* colorDetection_;

            CurveRunning* curveRunning_;
            LineTrace* lineTrace_;
            StraightRunning* straightRunning_;

            LineTraceEdge startEdge_;
            LineTraceEdge endEdge_;

            int correction_;


        public:
            //コンストラクタ
            Catching();

            /**
             * @brief 台座上走行
             *
             * @param digree カーブする角度(0<=digree<=360)
             * @return true:走行終了,false:走行中
             */
            bool run(int digree);

        private:

    };
}
#endif
