#ifndef ETSUMO_NEO_H
#define ETSUMO_NEO_H

#include "IStrategy.h"
#include "../drive/LineTrace.h"
#include "../drive/StraightRunning.h"
#include "../detection/ColorDetection.h"
#include "../detection/ObjectDetection.h"
#include "../detection/RightAngledDetection.h"
#include "../measurement/TimeMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "../measurement/Train.h"
#include "../drive/PolarRunning.h"
#include "../drive/ForcingOutRunning.h"
#include "../device/Arm.h"
#include <vector>

#define CENTER_TO_BLOCK_LENGTH 180  // 真ん中からブロック置き場に行く時の走る距離
#define SLIP_DEGREE10 15    // 1回ブロック置き場に行って帰ってきたごとにずれる角度の10倍
namespace strategy{
    class ETSumoNeo : public IStrategy{
    private:
        // 土俵のブロック置き場の色
        // TODO: 土俵の配置に合わせて変更
        colorid_t blockPlaceColors_[8] = {
            COLOR_YELLOW,   // 入り口から見て左奥
            COLOR_RED,      // 入り口から見て左手前
            COLOR_BLUE,     // 入り口から見て右手前
            COLOR_GREEN,    // 入り口から見て右奥

            // 土俵2つめ
            COLOR_YELLOW,   // 左奥
            COLOR_RED,      // 左手前
            COLOR_BLUE,     // 右手前
            COLOR_GREEN,    // 右奥
        };

        //ET相撲Neoの走行状態
        enum class StrategyPhase{
            INIT,
            LINE_TRACE,
            LINE_TRACE_LITTLE,

            STOP_ENTRY,
            STOP_EXIT,
            STOP_CENTER,

            WAIT_1_SEC,
            WAIT_2_SEC,
            TURN_LITTLE,
            CLIMB,
            LINE_RETURN,
            STABILIZE,
            TO_RIGHT_ANGLE,
            TO_CENTER,
            TO_BLOCK1,
            TO_LEFTEDGE,
            BACK1,
            TO_BLOCK2,
            BACK2,
            TO_BLOCK3,
            BACK3,
            TO_BLOCK4,
            BACK4,
            TO_LINE_RETURN,

            FORCING_OUT,
            SEE_BLOCK,
            ARM_UP,
            ARM_HOLD,
            ARM_NORMAL,

            NEXT_STAGE,
            EXIT,
        };


        //難所攻略手順
        std::vector<StrategyPhase> strategyProcedure_{
            StrategyPhase::INIT,             //車体角度保存
            StrategyPhase::LINE_TRACE,       //土俵を向くまでライントレース
            StrategyPhase::LINE_TRACE_LITTLE,//土俵まで距離が足りないので追加
            StrategyPhase::STABILIZE,
            StrategyPhase::STOP_ENTRY,       //新幹線検知するまで停止

            StrategyPhase::CLIMB,            //登壇

            StrategyPhase::TO_LEFTEDGE,     // 左エッジに行くように旋回する
            StrategyPhase::LINE_RETURN,     // ライン復帰する
            StrategyPhase::STABILIZE,       // 車体角度を安定させる
            StrategyPhase::INIT,            //車体角度保存
            StrategyPhase::TO_RIGHT_ANGLE,  // 直角検知するまで進む
            StrategyPhase::TO_CENTER,       // 土俵の真ん中に移動する

            // 土俵1つめの相撲
            // 1つめのブロック(入り口から見て左奥)
            StrategyPhase::ARM_HOLD,        // アームを上に向ける
            StrategyPhase::TO_BLOCK1,       // 1つ目のブロックに行く
            StrategyPhase::ARM_UP,          // 色を見るためにアームを上げる
            StrategyPhase::SEE_BLOCK,       // 色を見る
            StrategyPhase::ARM_HOLD,        // ブロックをつかむ
            StrategyPhase::FORCING_OUT,     // 寄り切りか押出をする
            StrategyPhase::BACK1,

            // 2つめのブロック(入り口から見て左手前)
            StrategyPhase::ARM_HOLD,
            StrategyPhase::TO_BLOCK2,
            StrategyPhase::ARM_UP,
            StrategyPhase::SEE_BLOCK,
            StrategyPhase::ARM_HOLD,
            StrategyPhase::FORCING_OUT,
            StrategyPhase::BACK2,

            // 3つ目のブロック(入り口から見て右手前)
            StrategyPhase::ARM_HOLD,
            StrategyPhase::TO_BLOCK3,
            StrategyPhase::ARM_UP,
            StrategyPhase::SEE_BLOCK,
            StrategyPhase::ARM_HOLD,
            StrategyPhase::FORCING_OUT,
            StrategyPhase::BACK3,

            // 4つめのブロック(入り口から見て右奥)
            StrategyPhase::ARM_HOLD,
            StrategyPhase::TO_BLOCK4,
            StrategyPhase::ARM_UP,
            StrategyPhase::SEE_BLOCK,
            StrategyPhase::ARM_HOLD,
            StrategyPhase::FORCING_OUT,
            StrategyPhase::BACK4,

            StrategyPhase::ARM_NORMAL,
            StrategyPhase::TO_LINE_RETURN,  // ライン復帰に渡せる場所に移動
            StrategyPhase::TO_LEFTEDGE,     // 左エッジに移動
            StrategyPhase::LINE_RETURN,     // ライン復帰
            StrategyPhase::STABILIZE,       // 角度を安定させる
            StrategyPhase::INIT,            // 車体角度保存
            StrategyPhase::TO_RIGHT_ANGLE,  // 直角検知まで移動

            // 次の土俵に移動する
            StrategyPhase::STOP_CENTER,     // 中央の新幹線を待つ
            StrategyPhase::NEXT_STAGE,      // 次の土俵に移動
            StrategyPhase::TO_LEFTEDGE,     // 左エッジに移動
            StrategyPhase::LINE_RETURN,     // ライン復帰
            StrategyPhase::STABILIZE,
            StrategyPhase::INIT,            // 車体角度保存
            StrategyPhase::TO_RIGHT_ANGLE,
            StrategyPhase::TO_CENTER,

            // 土俵2つめの相撲
            // 2つめの左奥
            StrategyPhase::ARM_HOLD,
            StrategyPhase::TO_BLOCK1,
            StrategyPhase::ARM_UP,
            StrategyPhase::SEE_BLOCK,
            StrategyPhase::ARM_HOLD,
            StrategyPhase::FORCING_OUT,
            StrategyPhase::BACK1,

            // 2つめの左手前
            StrategyPhase::ARM_HOLD,
            StrategyPhase::TO_BLOCK2,
            StrategyPhase::ARM_UP,
            StrategyPhase::SEE_BLOCK,
            StrategyPhase::ARM_HOLD,
            StrategyPhase::FORCING_OUT,
            StrategyPhase::BACK2,

            // 2つめの右手前
            StrategyPhase::ARM_HOLD,
            StrategyPhase::TO_BLOCK3,
            StrategyPhase::ARM_UP,
            StrategyPhase::SEE_BLOCK,
            StrategyPhase::ARM_HOLD,
            StrategyPhase::FORCING_OUT,
            StrategyPhase::BACK3,

            // 2つめの右奥
            StrategyPhase::ARM_HOLD,
            StrategyPhase::TO_BLOCK4,
            StrategyPhase::ARM_UP,
            StrategyPhase::SEE_BLOCK,
            StrategyPhase::ARM_HOLD,
            StrategyPhase::FORCING_OUT,
            StrategyPhase::BACK4,

            StrategyPhase::ARM_NORMAL,
            StrategyPhase::TO_LINE_RETURN,
            StrategyPhase::TO_LEFTEDGE,
            StrategyPhase::LINE_RETURN,
            StrategyPhase::STABILIZE,
            StrategyPhase::INIT,            // 車体角度保存
            StrategyPhase::TO_RIGHT_ANGLE,
            StrategyPhase::STOP_EXIT,

            StrategyPhase::EXIT,            // 土俵から出る
        };

        unsigned int procedureNumber_ = 0;
        unsigned int blockPlaceNum_ = 0;

        //走行
        drive::LineTrace* linetrace_;
        drive::PolarRunning polar_;
        drive::ForcingOutRunning forcingOut_;
        drive::StraightRunning* straightRunning_;

        device::Arm* arm_;

        //検知
        detection::ObjectDetection* objectDetection_;
        detection::RightAngledDetection* rightAngledDetection_;
        detection::ColorDetection colorDetection_;

        //計測
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;
        measurement::Train* train_;


        //距離検知の設定などを一度だけ行うためのフラグ
        bool hasExecutedPhase_;

        //難所クリアしたかどうか
        bool strategySuccess_;

        //ライントレースのリセットを行ったかどうか
        bool isLineTraceReset_;

        bool colorDetected_ = false;
        bool isRight_ = false;

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

        bool polarToBlock(int degree10);
    };
}

#endif
