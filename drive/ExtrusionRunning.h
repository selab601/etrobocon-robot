#ifndef EXTRUSION_RUNNING_H_
#define EXTRUSION_RUNNING_H_

#include "straightRunning.h"
#include "../measurement/DistanceMeasurement.h"
#include "../measurement/TimeMeasurement.h"

namespace drive{
    class ExtrusionRunning{

        private:
            //直線走行
            StraightRunning* straightRunning_;

            //距離検知
            measurement::DistanceMeasurement* distanceMeasurement_;

            //時間検知
            measurement::TimeMeasurement* timeMeasurement_;

            //走行状態
            enum RunningState
            {
                INIT,       //初期状態
                FORWARD,    //前進
                STOP,       //一時停止
                BACKWARD,   //後退
                END         //終了
            };

            RunningState runningState_;

            //押し出しスピード
            int extrusionSpeed_;

            //押し出し距離
            int extrusionDistance_;

        public:
            /**
             * @brief コンストラクタ
             * @param extrusionSpeed 押し出すスピード
             * @param extrusionDistance 押し出す距離
             */
            ExtrusionRunning(int extrusionSpeed, int extrusionDistance);

            /**
             * @brief 押し出し走行を行う
             * @return 走行終了:true,走行中:false
             */
            bool run();
    };
};
#endif
