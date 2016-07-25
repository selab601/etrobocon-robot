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
            STOP_2,     //
            WAIT_3,     //
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
            //TURN_0,
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

        std::vector<SumoPhase> sumoProcedure_;


        StrategyPhase strategyPhase_;
        ExtrusionPhase extrusionPhase_;
        //SumoPhase sumoPhase_;
        Hoshitori hoshitori_;

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


        bool hasExecutedSumoPhase_;

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
         * @return 攻略完了：true,攻略中：false
         */
        bool capture();

    private:
        /**
         * @brief 相撲を行う
         * @details 星取の色によって振る舞いを変える
         * @return 相撲終了:true,攻略中:false
         */
        bool executeSumo(SumoPhase sumoPhase);

        void setProcedure();

        /**
         * @brief 指定した星取の力士を押し出す
         * @details 中央線からスタートし、中央線で終了する、
         * 左側(赤・黄)と右側(青・緑)はそれぞれ同じ動作をする
         *
         * @param hoshitori 押し出す力士の星取
         * @return 押し出し終了:true,攻略中:false
         */
        bool extrusion(Hoshitori hoshitori);

        /**
         * @brief 星取検知
         * @details 星取の赤、青、黄、緑のいずれかを検知する、
         * 引数をtureとすることで取得した星取を保存できる
         *
         * @param getHoshitori 星取を保存するかどうか
         * @return 赤、青、黄、緑だった場合:true
         */
        bool hoshitoriDetection(bool saveHoshitori = false);
    };
}

#endif
