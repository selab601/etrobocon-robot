#ifndef AIREAD_H
#define AIREAD_H

#define NUM_ANA 7 //デバック用
#define NUM_DEG 0 //デバック用

#include "IStrategy.h"
#include "../drive/LineTrace.h"
#include "../drive/StraightRunning.h"
#include "../detection/ColorDetection.h"
#include "../detection/ObjectDetection.h"
#include "../detection/RightAngledDetection.h"
#include "../detection/LineDetection.h"
#include "../measurement/TimeMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "../measurement/Train.h"
#include "../drive/PolarRunning.h"
#include "../drive/ForcingOutRunning.h"
#include "../device/Arm.h"
#include <vector>

#define CENTER_TO_BLOCK_LENGTH 180  // 真ん中からブロック置き場に行く時の走る距離
#define SLIP_DEGREE10 20    // 1回ブロック置き場に行って帰ってきたごとにずれる角度の10倍
namespace strategy{
    class AIRead : public IStrategy{

int number_analog=NUM_ANA;
int number_degital=NUM_DEG;

    private:
        // 土俵のブロック置き場の色
        // TODO: 土俵の配置に合わせて変更
        colorid_t blockPlaceColors_[8] = {
            COLOR_RED,      // 入り口から見て左奥
            COLOR_YELLOW,   // 入り口から見て左手前
            COLOR_GREEN,    // 入り口から見て右手前
            COLOR_BLUE,     // 入り口から見て右奥


            COLOR_RED,      // 入り口から見て左奥
            COLOR_YELLOW,   // 入り口から見て左手前
            COLOR_GREEN,    // 入り口から見て右手前
            COLOR_BLUE,     // 入り口から見て右奥
        };

        //ET相撲Neoの走行状態
        enum class StrategyPhase{
            INIT,
            LINE_TRACE,
            LINE_TRACE_LITTLE,

            STOP_ENTRY,
            STOP_CENTER,

            WAIT_1_SEC,
            WAIT_2_SEC,
            TURN_LITTLE,
            CLIMB,
            LINE_RETURN,
            STABILIZE,
            TO_RIGHT_ANGLE,
            TO_CENTER,
            TURN_STRAIGHT,
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

            SEE_BLOCK,
            ARM_UP,
            ARM_HOLD,
            ARM_NORMAL,

            NEXT_STAGE,

            TURN_LEFT_90,
            FORWARD_10,
            FORWARD_20,
            FORWARD_50,
            FORWARD_100,
            FORWARD_150,
            TURN_LEFT_180,
            FORWARD_300,

            LINE_TRACE_25R, 
            LINE_TRACE_50R,
            LINE_TRACE_100R,
            LINE_TRACE_300R,
            LINE_TRACE_350R,
            LINE_TRACE_150R,
            LINE_TRACE_25L, 
            LINE_TRACE_50L,
            LINE_TRACE_100L,
            LINE_TRACE_300L,
            LINE_TRACE_150L,
            LINE_TRACE_350L,
            BACK_10,            
            BACK_50,
            BACK_100,
            BACK_150,
            BACK_300,
            READ_0,
            READ_1,
            READ_2,
            READ_3,
            READ_4,
            READ_5,
            READ_6,
            READ_7,
            READ_8,
            READ_9,
            READ_10,
            READ_11,
            TURN_RIGHT_90,
            JUDGE_D,
            JUDGE_A,
            B,
            C,
            D,
            E,
            F,

        };


        //難所攻略手順
        
        std::vector<StrategyPhase> strategyProcedure_{
           //デジタル検知
           StrategyPhase::TURN_LEFT_90,
           StrategyPhase::FORWARD_100,
           StrategyPhase::INIT,
           StrategyPhase::TURN_RIGHT_90,
           StrategyPhase::FORWARD_150,
           StrategyPhase::INIT,
           StrategyPhase::READ_0,//デジタル1検知
           StrategyPhase::READ_5,//デジタル6検知
           StrategyPhase::FORWARD_50,
           StrategyPhase::INIT,
           StrategyPhase::TURN_LEFT_90,
           StrategyPhase::FORWARD_150,
           StrategyPhase::INIT,
           StrategyPhase::TURN_LEFT_90,
           StrategyPhase::READ_6,//デジタル7検知
           StrategyPhase::READ_1,//デジタル2検知
           StrategyPhase::INIT,
           StrategyPhase::TURN_RIGHT_90,
           StrategyPhase::FORWARD_150,
           StrategyPhase::TURN_RIGHT_90,
           StrategyPhase::INIT,
           StrategyPhase::FORWARD_150,
           StrategyPhase::TURN_RIGHT_90,
           StrategyPhase::INIT,
           StrategyPhase::READ_4,//デジタル5検知
           StrategyPhase::READ_3,//デジタル4検知
           StrategyPhase::READ_2,//デジタル3検知
           StrategyPhase::INIT,
           StrategyPhase::TURN_LEFT_90,
           //デジタル判別　パターンから数字を決定変数NUMDEGに代入
           //StrategyPhase::JUDGE_D,
           //アナログへ
           //25cm前へ
           StrategyPhase::FORWARD_150,
           StrategyPhase::FORWARD_100,  
          // StrategyPhase::FORWARD_100, 
           //anastart
           StrategyPhase::FORWARD_50,
           StrategyPhase::FORWARD_20,
           StrategyPhase::TURN_LEFT_90,
           StrategyPhase::FORWARD_100,
           StrategyPhase::FORWARD_20,
           StrategyPhase::FORWARD_20,
           StrategyPhase::READ_7,//アナログ0検知
           StrategyPhase::FORWARD_100,
           StrategyPhase::FORWARD_20,
           StrategyPhase::READ_8,//ana1
           StrategyPhase::FORWARD_20,
           StrategyPhase::FORWARD_20,      
           StrategyPhase::FORWARD_20,
           StrategyPhase::READ_9,//ana2
           StrategyPhase::BACK_50,
           StrategyPhase::BACK_10,
           StrategyPhase::TURN_RIGHT_90,
           StrategyPhase::FORWARD_50,
           StrategyPhase::FORWARD_20,
           StrategyPhase::FORWARD_10,
           StrategyPhase::READ_10,//ana3
           StrategyPhase::FORWARD_50,
           StrategyPhase::FORWARD_20,
           StrategyPhase::FORWARD_10,
           StrategyPhase::READ_11,//ana4
         
                //向きを変え、移動する            
           //アナログ検知
           //アナログ判別

            StrategyPhase::TURN_LEFT_90,
            StrategyPhase::FORWARD_100,
            StrategyPhase::FORWARD_100,
            StrategyPhase::FORWARD_100,
            StrategyPhase::JUDGE_D,
            StrategyPhase::JUDGE_A,
            StrategyPhase::B,
            StrategyPhase::C,
            StrategyPhase::D,
            StrategyPhase::E,
            StrategyPhase::F,

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
        detection::LineDetection* lineDetection_;

        //計測
        measurement::DistanceMeasurement* distanceMeasurement_;
        measurement::TimeMeasurement* timeMeasurement_;
        measurement::BodyAngleMeasurement* bodyAngleMeasurement_;
        measurement::Train* train_;
        drive::PivotTurn pivot;


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
        AIRead();

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
