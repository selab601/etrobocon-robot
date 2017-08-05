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
#include "../device/SonarSensor.h"
#include <vector>

namespace strategy{
    class Prize : public IStrategy{
    private:
        //懸賞運びの走行状態
        enum class StrategyPhase{
            INIT,
            APPROACH1,
            APPROACH2,
            SET_DIFFANGLE1,
            TURN_SOUTH,
            ARM_DOWN,
            ACCESS1,
            PICKUP_PRIZE,
            YOISHO,
            BACK1,
            SET_DIFFANGLE2,
            TURN_EAST,
            TURN_WEST,
            STOP_EXIT,
            WAIT_1_SEC,
            LEAVE_AREA_BACK,
            LEAVE_AREA1,
            LEAVE_AREA2,
            RECOVER,
            APPROACH2,
            ADJUST_ANGLE_SOUTH,
            PUTDOWN_PRIZE,
            BACK2,
            SET_DIFFANGLE4,
            TURN_EAST2,
            RETURN_LINE,
            EXIT,
            ARM,
        };


        //難所攻略手順
        std::vector<StrategyPhase> strategyProcedure_{
            //StrategyPhase::ARM,
            StrategyPhase::INIT,
            StrategyPhase::SET_DIFFANGLE1,
            StrategyPhase::TURN_SOUTH, 
            StrategyPhase::APPROACH1,
            StrategyPhase::SET_DIFFANGLE1,
            StrategyPhase::TURN_SOUTH,
            StrategyPhase::APPROACH2,
            StrategyPhase::SET_DIFFANGLE1,
            StrategyPhase::TURN_SOUTH,
            StrategyPhase::ARM_DOWN,
            StrategyPhase::ACCESS1,
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
            StrategyPhase::APPROACH2,
            StrategyPhase::PUTDOWN_PRIZE,
            StrategyPhase::BACK2,
            StrategyPhase::SET_DIFFANGLE4,
            StrategyPhase::TURN_EAST2,
            StrategyPhase::EXIT,
        };

        unsigned int procedureNumber_ = 0;

        //検知
        detection::LineDetection* lineDetection_;
        detection::ObjectDetection* objectDetection_;

        device::Arm* arm_;
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
