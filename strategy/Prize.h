#ifndef PRIZE_H
#define PRIZE_H

#include "IStrategy.h"
#include "../detection/LineDetection.h"
#include "../detection/ObjectDetection.h"
#include "../drive/LineTrace.h"
#include "../drive/StraightRunning.h"
#include "../measurement/TimeMeasurement.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "../measurement/Train.h"
#include "../drive/PolarRunning.h"
#include "../device/Arm.h"
#include "../device/GyroSensor.h"
#include "../device/SonarSensor.h"
#include <vector>

namespace strategy{
    class Prize : public IStrategy{
    private:
        //懸賞運びの走行状態
        enum class StrategyPhase{
            INIT,
            EXIT,

            //回転
            SET_DIFFANGLE1,
            SET_DIFFANGLE2,
            SET_DIFFANGLE4,
            TURN_EAST,
            TURN_EAST2,
            TURN_WEST,
            TURN_SOUTH,
            TURN_SOUTH2,
            ADJUST_ANGLE_SOUTH,

            //移動
            APPROACH5CM,
            APPROACH10CM,
            APPROACH12CM,
            APPROACH16CM,
            APPROACH16CM_SONAR,
            APPROACH_KENSHO,
            BACK1,
            BACKTOLINE,
            LEAVE_AREA1,
            LEAVE_AREA2,
            RECOVER,
            RECOVER_GYRO,
            RETURN_LINE,

            //アーム
            ARM_DOWN,
            PICKUP_PRIZE,
            PUTDOWN_PRIZE,
            YOISHO,

            STOP_EXIT,
            WAIT_1_SEC,

            TEST,
            FALS,
        };


        //難所攻略手順
        std::vector<StrategyPhase> strategyProcedure_{
            StrategyPhase::INIT,
            StrategyPhase::SET_DIFFANGLE1,
            StrategyPhase::TURN_SOUTH,
            StrategyPhase::ARM_DOWN,
            StrategyPhase::APPROACH10CM,
            StrategyPhase::SET_DIFFANGLE1,
            StrategyPhase::TURN_SOUTH,
            StrategyPhase::APPROACH16CM,
            StrategyPhase::SET_DIFFANGLE1,
            StrategyPhase::TURN_SOUTH,
            StrategyPhase::PICKUP_PRIZE,

            StrategyPhase::BACK1,
            StrategyPhase::SET_DIFFANGLE2,
            StrategyPhase::TURN_EAST,
            StrategyPhase::STOP_EXIT,
            StrategyPhase::LEAVE_AREA1,
            StrategyPhase::LEAVE_AREA2,
            //StrategyPhase::WAIT_1_SEC,
            StrategyPhase::RECOVER,

            StrategyPhase::SET_DIFFANGLE1,
            StrategyPhase::TURN_SOUTH,
            StrategyPhase::APPROACH16CM,
            StrategyPhase::PUTDOWN_PRIZE,
            
            StrategyPhase::BACKTOLINE,
            StrategyPhase::SET_DIFFANGLE4,
            StrategyPhase::TURN_EAST2,
            StrategyPhase::EXIT,
        };

        unsigned int procedureNumber_ = 0;

        //検知
        detection::LineDetection* lineDetection_;
        detection::ObjectDetection* objectDetection_;

        device::Arm* arm_;
        device::GyroSensor* gyro_;
        device::SonarSensor* sonar_;

        //走行
        drive::LineTrace* linetrace_;
        drive::PolarRunning polar_;
        drive::StraightRunning* straightRunning_;

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

        bool isRight_ = false;

    public:
        //コンストラクタ
        Prize();

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
