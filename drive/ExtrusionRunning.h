#ifndef EXTRUSION_RUNNING_H_
#define EXTRUSION_RUNNING_H_

#include "straightRunning.h"
#include "../measurement/DistanceMeasurement.h"

namespace drive{
    class ExtrusionRunning{

        private:
            //直線走行
            StraightRunning* straightRunning_;

            //距離検知
            measurement::DistanceMeasurement* distanceMeasurement_;

            //走行状態
            enum RunningState
            {
                INIT,       //初期化
                FORWARD,    //前進
                BACKWARD    //後退
            };

            RunningState runningState_;

        public:
            /**
             * @brief 押し出し走行を行う
             * @param speed (0 < speed ≦ 100) スピード
             * @param distance (> 0)  押し出す距離
             * @return 走行終了:true,走行中:false
             */
            bool run(int speed,int distance);

            //コンストラクタ
            ExtrusionRunning();

        private:
            /**
             * @brief 指定距離走行
             * @param speed (0 < speed ≦ 100) スピード
             * @return 走行終了:true,走行中:false
             */
            bool specifiedDistanceRunning(int speed);
    };
};
#endif
