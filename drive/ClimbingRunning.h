#ifndef CLIMBING_RUNNING_H_
#define CLIMBING_RUNNING_H_

#include "straightRunning.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/CountMeasurement.h"
#include "../device/Arm.h"

#define CLIMB_ARM_TARGET_COUNT 50 //レールと土俵に当たらず走行できるアームの回転量

namespace drive{
    class ClimbingRunning{

        private:
            //直線走行
            StraightRunning* straightRunning_;

            //距離検知
            measurement::DistanceMeasurement* distanceMeasurement_;

            //モーター
            device::Arm* arm_;

            //走行状態
            enum class RunningState
            {
                ARM_UP,     //アームをあげる
                CLIMB,      //登壇
                ARM_DOWN,   //アームを戻す
                END         //終了
            };

            RunningState runningState_;

        public:

            //コンストラクタ
            ClimbingRunning();

            /**
             * @brief 登壇を行う
             * @param speed (0 < speed ≦ 100) スピード
             * @param distance (> 0)  登壇する距離
             * @return 登壇終了:true,走行中:false
             */
            bool run(int speed,int distance);
    };
};
#endif
