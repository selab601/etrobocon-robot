#ifndef ETSUMO_NEO_H
#define ETSUMO_NEO_H

#include "IStrategy.h"
#include "../drive/PushingOutRunning.h"
#include "../drive/ClimbingRunning.h"
#include "../drive/StraightRunning.h"
#include "../drive/PivotTurn.h"
#include "../drive/LineTrace.h"
#include "../drive/CurveRunning.h"
#include "../drive/Efforts.h"
#include "../detection/LineDetection.h"
#include "../detection/ColorDetection.h"
#include "../detection/ObjectDetection.h"
#include "../detection/RightAngledDetection.h"
#include "../measurement/TimeMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include <vector>

namespace strategy{
    class ETSumoNeo : public IStrategy{
    private:
        //ET相撲Neoの走行状態
        enum class StrategyPhase{
            INIT,
            LINE_TRACE,
            STOP,
            WAIT_1_SEC,
            WAIT_2_SEC,
            TURN_LITTLE,
            CLIMB,
            TURN_TO_SIDE,
            BACK_TO_LINE,
            STRAIGHT_4_CM,
            STRAIGHT_2_CM,
            TURN_TO_DOWN,
            TURN_RIGHT_90,
            TURN_LEFT_90,
            DOWN_STAGE,
            UPPER_STAGE,
            FIRST_EFFORTS,
            SECOND_EFFORTS,
            THIRD_EFFORTS,
            FOURTH_EFFORTS,
            ACROSS_LINE,
            CURVE_TOP,
            NEXT_STAGE,
        };


        //難所攻略手順
        std::vector<StrategyPhase> strategyProcedure_{
            StrategyPhase::INIT,             //車体角度保存
            StrategyPhase::LINE_TRACE,       //土俵を向くまでライントレース
            StrategyPhase::STOP,             //新幹線検知するまで停止
            StrategyPhase::WAIT_1_SEC,       //検知後に待つ
            StrategyPhase::TURN_LITTLE,      //すこし旋回
            StrategyPhase::CLIMB,            //登壇
            StrategyPhase::WAIT_1_SEC,       //登壇後に機体が落ち着くまで待つ
            StrategyPhase::TURN_TO_SIDE,     //横を向く
            StrategyPhase::BACK_TO_LINE,     //中央線までバック
            StrategyPhase::STRAIGHT_4_CM,    //4cm直進
            StrategyPhase::TURN_TO_DOWN,     //下を向くように旋回
            StrategyPhase::DOWN_STAGE,       //下段に移動
            StrategyPhase::STRAIGHT_2_CM,    //すこし進む
            StrategyPhase::TURN_RIGHT_90,    //右に旋回
            StrategyPhase::FIRST_EFFORTS,    //一回目の取組
            StrategyPhase::ACROSS_LINE,      //線を超える
            StrategyPhase::SECOND_EFFORTS,   //二回目の取組
            StrategyPhase::INIT,             //車体角度をリセット
            StrategyPhase::CURVE_TOP,        //上を向く
            StrategyPhase::UPPER_STAGE,      //上段に移動
            StrategyPhase::ACROSS_LINE,      //線を超える
            StrategyPhase::TURN_RIGHT_90,    //右に旋回
            StrategyPhase::THIRD_EFFORTS,    //三回目の取組
            StrategyPhase::ACROSS_LINE,      //線を超える
            StrategyPhase::FOURTH_EFFORTS,   //四回目の取組
            StrategyPhase::TURN_LEFT_90,     //左に旋回
            StrategyPhase::BACK_TO_LINE,     //近すぎるのでバック
            StrategyPhase::STOP,             //新幹線検知まで停止
            StrategyPhase::WAIT_2_SEC,       //通過するまで待つ

            StrategyPhase::NEXT_STAGE,       //次のステージへ

            StrategyPhase::TURN_TO_SIDE,     //横を向く
            StrategyPhase::BACK_TO_LINE,     //中央線までバック
            StrategyPhase::STRAIGHT_4_CM,    //4cm直進
            StrategyPhase::TURN_TO_DOWN,     //下を向くように旋回
            StrategyPhase::DOWN_STAGE,       //下段に移動
            StrategyPhase::STRAIGHT_2_CM,    //すこし進む
            StrategyPhase::TURN_RIGHT_90,    //右に旋回
            StrategyPhase::FIRST_EFFORTS,    //五回目の取組
            StrategyPhase::ACROSS_LINE,      //線を超える
            StrategyPhase::SECOND_EFFORTS,   //六回目の取組
            StrategyPhase::INIT,             //車体角度をリセット
            StrategyPhase::CURVE_TOP,        //上を向く
            StrategyPhase::UPPER_STAGE,      //上段に移動
            StrategyPhase::ACROSS_LINE,      //線を超える
            StrategyPhase::TURN_RIGHT_90,    //右に旋回
            StrategyPhase::THIRD_EFFORTS,    //七回目の取組
            StrategyPhase::ACROSS_LINE,      //線を超える
            StrategyPhase::FOURTH_EFFORTS,   //八回目の取組
            StrategyPhase::TURN_LEFT_90,     //左に旋回
            StrategyPhase::BACK_TO_LINE,
            StrategyPhase::STOP,             //新幹線検知まで停止
            StrategyPhase::WAIT_2_SEC,       //通過するまで待つ
            StrategyPhase::NEXT_STAGE,//仮置き
        };


        //走行
        drive::ClimbingRunning* climbingRunning_;
        drive::StraightRunning* straightRunning_;
        drive::PivotTurn* pivotTurn_;
        drive::CurveRunning* curveRunning_;
        drive::LineTrace* linetrace_;
        drive::Efforts* efforts_;

        //検知
        detection::LineDetection* lineDetection_;
        detection::ObjectDetection* objectDetection_;
        detection::RightAngledDetection* rightAngledDetection_;

        //計測
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;


        //距離検知の設定などを一度だけ行うためのフラグ
        bool hasExecutedPhase_;

        //難所クリアしたかどうか
        bool strategySuccess_;

        //ライントレースのリセットを行ったかどうか
        bool isLineTraceReset_;


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


        /**
         * @brief ライン検知するまで旋回する
         *
         * @param isRight 右旋回かどうか
         * @param speed 旋回スピード
         *
         * @return 終了:true,移動中:false
         */
        bool turn(bool isRight,int speed);

        /**
         * @brief 上段に向けてカーブ走行する
         *
         * @param isRight 右カーブかどうか
         * @param speed カーブスピード
         *
         * @return 終了:true,移動中:false
         */
        bool curve(bool isRight,int speed);


        /**
         * @brief 時間検知を開始する
         * @details setBase,setTargetを同時に行い、フラグ管理も行う
         * @param time 検知する時間
         * @param flagChange フラグを変更するかどうか
         */
        void startTimeMeasurement(int time,bool flagChange = true);

        /**
         * @brief LineTrace::reset()を一度だけ実行する
         */
        void lineTraceReset();
    };
}

#endif
