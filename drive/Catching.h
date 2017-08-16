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

#define CATCHING_PWM 20//キャッチするときの旋回スピード
#define CATCHING_180_PWM 20//180度用の旋回スピード
#define CATCHING_LINETRACE_PWM 30//ライントレースのスピード
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
                START_LINE_TRACE,       //色検知までライントレース
                STRAIGHT_LITTLE,        //タイヤの中心を円周上まで移動させる
                PIVOT_FIRST,            //引数の半分の角度旋回する（１回目）
                STRAIGHT,               //直進走行(角度が大きい場合)
                PIVOT_SECOND,           //引数の半分の角度旋回する（２回目）
                CALC_DISTANCE,          //エッジに応じた距離を計算する
                END_LINE_TRACE,         //カーブ後のライントレース
                TURN_90,                //９０度カーブ
                TURN_270,               //２７０度カーブ
                STRAIGHT_TREAD_DISTANCE,//走行体のトレッドの距離走行
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

            int dstDegree_;//現在地から目的地までの角度
            int runningDistance_;//カーブ後に走る距離

        public:
            //コンストラクタ
            Catching();

            /**
             * @brief 台座上走行
             *
             * @param dstMm 目的地ラインの長さ[mm]
             * @param degree カーブする角度(-180<=degree<=180)
             * @return true:走行終了,false:走行中
             */
            bool run(int dstMm, int degree);

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
