#ifndef CATCHING_H
#define CATCHING_H

#include "../measurement/BodyAngleMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/SelfPositionEstimation.h"
#include "./CurveRunning.h"
#include "./LineTrace.h"
#include "./StraightRunning.h"
#include "./PivotTurn.h"
#include "../detection/ColorDetection.h"
#include <stdlib.h>

#define CATCHING_PWM 20
#define CATCHING_LINETRACE_PWM 20
#define DAIZA_DIAMETER 100//円の直径[mm]
#define WHEEL_TO_COLOR_SENSOR 45//タイヤの中心からカラーセンサの中心までの距離[mm]
#define COLOR_DETECTION_DISTANCE 15//色検知中に動いてしまう距離(pwm20)[mm]
#define LINE_THICKNESS 20//ラインの太さ[mm]

namespace drive
{
    class Catching{
        private:
            //走行状態
            enum class Phase
            {
                START_LINE_TRACE,//色検知までライントレース
                STRAIGHT_LITTLE,
                PIVOT_FIRST,
                STRAIGHT,        //直進走行(角度が大きい場合)
                PIVOT_SECOND,
                CALC_DISTANCE,
                END_LINE_TRACE   //カーブ後のライントレース
            };

            Phase phase_ = Phase::START_LINE_TRACE;

            measurement::BodyAngleMeasurement* bodyAngleMeasurement_;
            measurement::DistanceMeasurement* distanceMeasurement_;
            measurement::SelfPositionEstimation* selfPositionEstimation_;

            detection::ColorDetection* colorDetection_;

            CurveRunning* curveRunning_;
            LineTrace* lineTrace_;
            StraightRunning* straightRunning_;
            PivotTurn* pivotTurn_;

            LineTraceEdge startEdge_;
            LineTraceEdge endEdge_;

            int correction_;
            int dstDegree_;//現在地から目的地までの角度
            int runningDistance_;//カーブ後に走る距離

        public:
            //コンストラクタ
            Catching();

            /**
             * @brief 台座上走行
             *
             * @param currentMm 現在のラインの長さ[mm]
             * @param dstMm 目的地ラインの長さ[mm]
             * @param degree カーブする角度(-180<=degree<=180)
             * @return true:走行終了,false:走行中
             */
            bool run(int currentMm, int dstMm, int degree);

            /**
             * @brief 現在運んでるブロックを台座の上に置く
             *
             * @param lineDistance 走行しているラインの距離
             * @return true:設置完了,false:設置中
             */
            bool putBlock(int lineDistance = 450);

        private:

    };
}
#endif
