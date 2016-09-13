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
#include "../device/Shippo.h"
#include "../device/Arm.h"
#include "../device/SonarSensor.h"
#include <vector>

namespace strategy{
    class PrizeTailVer : public IStrategy{
    private:
        enum class Phase{
            LINE_TRACE,
            LINE_TRACE2,
            DOWN_ARM,
            LINE_TRACE_UP_TO_PRIZE,
            TURN_LEFT,
            PUT_IN_LOWER_OF_PRIZE,
            LIFT_PRIZE,
            BACK_14CM,
            DOWN_PRIZE,
            DOWN_SHIPPO,
            PUT_AFTER_BACK,
            CURVE_AFTER_BACK,
            LEFT_90_ROTATION,
            CURVE_UP_TO_PRIZE_SIDE,
            STRAIGHT_1_CM,
            GET_PRIZE,
            LINE_TRACE_UP_TO_GOOL,
            LINE_TRACE_UP_TO_GOOL2,
            FINISHED,
        };

        //懸賞運びの攻略手順
        std::vector<Phase> phaseProcedure_{
            Phase::LINE_TRACE,
            Phase::LINE_TRACE2,
            Phase::DOWN_ARM,                    //アームを下げる
            Phase::LINE_TRACE_UP_TO_PRIZE,      //懸賞前までライントレース
            Phase::TURN_LEFT,                   //左側のアームで懸賞を押しちゃうことがあるので左に5度向ける
            Phase::PUT_IN_LOWER_OF_PRIZE,       //懸賞の下にアームを入れる
            Phase::LIFT_PRIZE,                  //懸賞を持ち上げる
            Phase::BACK_14CM,                   //懸賞を下ろす前に下がる
            Phase::DOWN_PRIZE,                  //懸賞を下ろす
            Phase::DOWN_SHIPPO,                 // しっぽをさげる
            Phase::PUT_AFTER_BACK,              //置いた後にバック
            Phase::LEFT_90_ROTATION,            //左90度旋回
            Phase::CURVE_UP_TO_PRIZE_SIDE,      //懸賞の横までカーブ
            Phase::CURVE_AFTER_BACK,            //カーブ後にバック
            Phase::LEFT_90_ROTATION,            //左90度カーブ
            Phase::GET_PRIZE,                   // しっぽで懸賞を取る
            Phase::LINE_TRACE_UP_TO_GOOL,       //ゴールまでライントレース
            Phase::LINE_TRACE_UP_TO_GOOL2,
            Phase::FINISHED                     // とまってしっぽふりふり
        };

        //走行
        drive::PivotTurn* pivotTurn_;
        drive::CurveRunning* curveRunning_;
        drive::StraightRunning* straightRunning_;
        drive::LineTrace* lineTrace_;

        //検知
        detection::ObjectDetection* objectDetection_;
        detection::LineDetection* lineDetection_;

        //計測
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;

        //デバイス
        device::Motors* motors_;

        //難所クリアしたかどうか
        bool strategySuccess_;

        //距離検知の設定などを一度だけ行うためのフラグ
        bool hasExecutedPhase_;

        //ライントレースのリセットを行ったかどうか
        bool isLineTraceReset_;
    public:
        //コンストラクタ
        PrizeTailVer();

        /**
         * @brief 懸賞運びを攻略する
         * @return 攻略完了:true,攻略中:false
         */
        bool capture();

    private:
        /**
         * @brief 懸賞運びの手順を実行
         *
         * @param phase 手順
         * @return 終了:true,攻略中:false
         */
        bool executePhase(Phase phase);

        /**
         * @brief 時間検知を開始する
         * @details setBaseTime(),setTargetTime(time)を同時に行う
         *
         * @param time 検知する時間
         */
        void startTimeMeasurement(int time);

        /**
         * @brief 距離検知を開始する
         * @details setTargetDistance(distance),startMeasurement()を同時に行う
         *
         * @param time 検知する距離
         */
        void startDistanceMeasurement(int distance);

        /**
         * @brief LineTrace::reset()を一度だけ実行する
         */
        void lineTraceReset();

    };
}

#endif
