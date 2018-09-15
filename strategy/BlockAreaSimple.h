#ifndef BLOCK_AREA_SIMPLE_H
#define BLOCK_AREA_SIMPLE_H

#include "IStrategy.h"
#include "../detection/LineDetection.h"
#include "../detection/RightAngledDetection.h"
#include "../detection/ObjectDetection.h"
#include "../drive/LineTrace.h"
#include "../drive/PivotTurn.h"
#include "../drive/PolarRunning.h"
#include "../drive/StraightRunning.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include <vector>

namespace strategy{
    class BlockAreaSimple : public IStrategy{
    private:
        //懸賞運びの走行状態
        enum class StrategyPhase{
            INIT,
            ENTRY,
            TOSSHIN,
            TO_BLUE,
            BACK,
            AVOID1,
            AVOID2,
            AVOID3,
            AVOID4,
            AVOID5,
            LINE_TRACE,
            SET_DIFFANGLE,
            FIX_ANGLE,
        };


        //難所攻略手順
        std::vector<StrategyPhase> strategyProcedure_{
            StrategyPhase::INIT,
            StrategyPhase::ENTRY,
            //StrategyPhase::TOSSHIN,
            StrategyPhase::TO_BLUE,
            StrategyPhase::BACK,
            StrategyPhase::AVOID1,
            StrategyPhase::AVOID2,
            StrategyPhase::AVOID3,
            StrategyPhase::AVOID4,
            StrategyPhase::AVOID5,
            StrategyPhase::LINE_TRACE,
            StrategyPhase::SET_DIFFANGLE,
            StrategyPhase::FIX_ANGLE,
        };

        unsigned int procedureNumber_ = 0;

        //検知
        detection::LineDetection* lineDetection_;
        detection::RightAngledDetection* rightAngledDetection_;

        //走行
        drive::LineTrace* lineTrace_;
        drive::PivotTurn* pivotTurn_;
        drive::PolarRunning polar_;
        drive::StraightRunning* straightRunning_;

        //計測
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;


        //距離検知の設定などを一度だけ行うためのフラグ
        bool hasExecutedPhase_;

        //難所クリアしたかどうか
        bool strategySuccess_;

        //ライントレースのリセットを行ったかどうか
        bool isLineTraceReset_;

        bool isRight_ = false;

    public:
        //コンストラクタ
        BlockAreaSimple();

        /**
         * @brief 懸賞運びを攻略する
         * @return 攻略完了:true,攻略中:false
         */
        bool capture();

    private:
        /**
         * @brief 難所の手順を実行
         * @param strategyPhase 難所の手順
         * @return 難所終了:true,攻略中:false
         */
        bool executeStrategy(StrategyPhase strategyPhase);

        /**
         * @brief LineTrace::reset()を一度だけ実行する
         */
        void lineTraceReset();
    };
}

#endif
