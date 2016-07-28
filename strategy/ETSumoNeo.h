#ifndef ETSUMO_NEO_H
#define ETSUMO_NEO_H

#include "IStrategy.h"
#include "../drive/ExtrusionRunning.h"
#include "../drive/ClimbingRunning.h"
#include "../drive/StraightRunning.h"
#include "../drive/PivotTurn.h"
#include "../drive/LineTrace.h"
#include "../drive/CurveRunning.h"
#include "../detection/LineDetection.h"
#include "../detection/ColorDetection.h"
#include "../detection/ObjectDetection.h"
#include "../detection/RightAngledDetection.h"
#include "../measurement/TimeMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include <vector>
#include <list>

#define SUMO_EXTRUSION_DISTANCE  100 //押し出し走行距離
#define SUMO_EXTRUSION_SPEED     20  //押し出しスピード

namespace strategy{
    class ETSumoNeo : public IStrategy{
    private:
        //ET相撲Neoの走行状態
        enum class StrategyPhase{
            HOSHITORI,  //星取り取得
            TURN_1,     //
            STRAIGHT,   //
            LINE_TRACE, //
            STOP_1,     //
            WAIT_1,     //
            CLIMB,      //
            WAIT_2,     //
            TURN_2,     //
            TURN_3,     //
            TURN_4,
            SUMO,       //
            GET_OF      //
        };

        //相撲全体の状態
        enum class SumoPhase{
            EXTRUSION_FIRST,    //
            EXTRUSION_SECOND,    //
            EXTRUSION_THIRD,    //
            TURN_1,         //
            TURN_2,         //
            UPPER_STAGE,    //
            ACROSS_LINE     //
        };

        //特定の力士を押し出す走行状態
        enum class ExtrusionPhase{
            LINE_TRACE_1,   //
            EXTRUSION,      //
            TURN_1,         //
            TURN_2,         //
            LINE_TRACE_2    //
        };

        //星取
        enum class Hoshitori{
            RED,        //赤
            BLUE,       //青
            YELLOW,     //黄
            GREEN,      //緑
            NONE        //未定義
        };

        //難所攻略手順
        std::vector<StrategyPhase> strategyProcedure_{
            StrategyPhase::HOSHITORI,
            StrategyPhase::TURN_1,
            StrategyPhase::STRAIGHT,
            StrategyPhase::LINE_TRACE,
            StrategyPhase::STOP_1,
            StrategyPhase::WAIT_1,
            StrategyPhase::TURN_2,
            StrategyPhase::CLIMB,
            StrategyPhase::WAIT_1,
            StrategyPhase::TURN_3,
            StrategyPhase::TURN_4,
            StrategyPhase::SUMO,
            StrategyPhase::TURN_2,
            StrategyPhase::STOP_1,
            StrategyPhase::WAIT_2,
            StrategyPhase::GET_OF
        };

        //相撲攻略手順(星取り赤・青)
        std::vector<SumoPhase> sumoProcedureRorB_ = {
            SumoPhase::EXTRUSION_FIRST,
            SumoPhase::ACROSS_LINE,
            SumoPhase::TURN_1,
            SumoPhase::UPPER_STAGE,
            SumoPhase::ACROSS_LINE,
            SumoPhase::TURN_2,
            SumoPhase::EXTRUSION_SECOND,
            SumoPhase::ACROSS_LINE,
            SumoPhase::EXTRUSION_THIRD,
            SumoPhase::TURN_1
        };

        //相撲攻略手順(星取り黄・緑)
        std::vector<SumoPhase> sumoProcedureYorG_{
            SumoPhase::EXTRUSION_FIRST,
            SumoPhase::ACROSS_LINE,
            SumoPhase::EXTRUSION_SECOND,
            SumoPhase::ACROSS_LINE,
            SumoPhase::TURN_1,
            SumoPhase::UPPER_STAGE,
            SumoPhase::ACROSS_LINE,
            SumoPhase::TURN_2,
            SumoPhase::EXTRUSION_THIRD,
            SumoPhase::TURN_2
        };



        //走行
        drive::ExtrusionRunning* extrusionRunning_;
        drive::ClimbingRunning* climbingRunning_;
        drive::StraightRunning* straightRunning_;
        drive::PivotTurn* pivotTurn_;
        drive::CurveRunning* curveRunning_;
        drive::LineTrace* linetrace_;


        //検知
        detection::LineDetection* lineDetection_;
        detection::ColorDetection* colorDetection_;
        detection::ObjectDetection* objectDetection_;
        detection::RightAngledDetection* rightAngledDetection_;

        //計測
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;




        ExtrusionPhase extrusionPhase_;
        Hoshitori hoshitori_;

        bool hasExecutedPhase_;
        bool hasExecutedSumoPhase_;

        bool strategySuccess_;

        //押し出す力士の色
        Hoshitori firstWrestlerColor_;
        Hoshitori secondWrestlerColor_;
        Hoshitori thirdWrestlerColor_;

        int angleTowardTop_;             //上に向かう角度
        int angleTowardWrestler_;        //力士に向かう角度
        drive::LineTraceEdge upperStageEdge_;   //上段に向かうライントレースのエッジ

    public:
        //コンストラクタ
        ETSumoNeo();

        /**
         * @brief ET相撲Neoを攻略する
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

        void setProcedure();

        /**
         * @brief 相撲を攻略する
         * @return 攻略完了:true,攻略中:false
         */
        bool captureSumo();

        /**
         * @brief 相撲の手順を実行
         * @details 星取の色によって振る舞いを変える
         * @param sumoPhase 相撲の手順
         * @return 相撲終了:true,攻略中:false
         */
        bool executeSumo(SumoPhase sumoPhase);

        //bool captureExtrusion();

        /**
         * @brief 指定した星取の力士を押し出す
         * @details 中央線からスタートし、中央線で終了する、
         * 左側(赤・黄)と右側(青・緑)はそれぞれ同じ動作をする
         *
         * @param wrestlerColor 押し出す力士の色
         * @return 押し出し終了:true,攻略中:false
         */
        bool extrusion(Hoshitori wrestlerColor);

        /**
         * @brief 星取検知
         * @details 星取の赤、青、黄、緑のいずれかを検知する、
         * 引数をtureとすることで取得した星取を保存できる
         *
         * @param getHoshitori 星取を保存するかどうか
         * @return 赤、青、黄、緑だった場合:true
         */
        bool hoshitoriDetection(bool saveHoshitori = false);

        void startDistanceMeasurement(int distance);

        void startTimeMeasurement(int time);
    };
}

#endif
