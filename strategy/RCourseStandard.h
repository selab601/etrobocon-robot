#ifndef _RCOURSE_STANDARD_H
#define _RCOURSE_STANDARD_H

#include "IStrategy.h"
//#include "device/Motors.h"
#include "drive/LineTrace.h"
#include "drive/CurveRunning.h"
#include "drive/StraightRunning.h"
#include "measurement/DistanceMeasurement.h"
#include "../measurement/BodyAngleMeasurement.h"
#include <vector>

namespace strategy{
    class RCourseStandard : public IStrategy{
        private:
            enum class Phase
            {
                STRAIGHT1,
                STRAIGHT2,
                STRAIGHT3,
                STRAIGHT4,
                STRAIGHT5,
                BEND1,
                BEND2,
                BEND3,
                BEND4,
                BEND5,
                BEND6,
                STOP,
                SLOWDOWN,
            };

            std::vector<Phase> phaseProcedure_{
                Phase::STRAIGHT1,
                Phase::BEND1,
                Phase::STRAIGHT2,
                Phase::BEND2,
                Phase::BEND3,
                Phase::STRAIGHT3,
                Phase::BEND4,
                Phase::STRAIGHT4,
                Phase::BEND5,
                Phase::STRAIGHT5,
                Phase::SLOWDOWN,
                //Phase::BEND6,
                //Phase::STOP,
            };

            drive::LineTrace* linetrace_;
            drive::CurveRunning* curveRunning_;
            drive::StraightRunning* straightRunning_;
            measurement::DistanceMeasurement* distanceMeasurement_;
            measurement::BodyAngleMeasurement bodyAngleMeasurement_;

            bool strategySuccess_;
            bool hasExecutedPhase_;
            bool isLineTraceReset_;

        public:
            //コンストラクタ
            RCourseStandard();

            bool capture();
        private:
            bool executePhase(Phase phase);
            void startDistanceMeasurement(int distance);
            void startAngleMeasurement();
            /**
             * @brief 一定距離ライントレース
             *
             * @param distance 走行距離
             * @param speed 走行スピード
             * @param edge ライントレースのエッジ
             * @return 一定距離走行:true,走行中:false
             */
            bool fixedDistanceLineTrace(int distance,int speed,drive::LineTraceEdge edge);
            /**
             * @brief 一定距離カーブライントレース
             *
             * @param distance 走行距離
             * @param deltaRad 角度？
             * @return 一定距離走行:true,走行中:false
             */
            bool fixedDistanceCurveLineTrace(int distance,int deltaRad);

            /**
             * @brief LineTrace::reset()を一度だけ実行する
             */
            void lineTraceReset();
        };
}

#endif
