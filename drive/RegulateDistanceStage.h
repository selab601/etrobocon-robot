#ifndef REGULATE_DISTANCE_STAGE_H_
#define REGULATE_DISTANCE_STAGE_H_

#include "../measurement/SelfPositionEstimation.h"
#include "../measurement/DistanceMeasurement.h"
#include "StraightRunning.h"
#include <cstdlib>

#define MAXBACKDISTANCE 40      //後退できる限界距離

namespace drive{
    class RegulateDistanceStage{
    private:
        StraightRunning* straightRunning_;
        measurement::SelfPositionEstimation* selfPositionEstimation_;
        measurement::DistanceMeasurement* distanceMeasurement_;
        int position_;          //x座標とy座標の最大値(現在地)
        int moveDistance_;      //距離調整に必要な移動距離
        int initialized_;
        int x_, y_;             //現在地座標
        int regulateDistance_;  //機体が最終的に位置する台座からの距離

    public:
        RegulateDistanceStage();
        /*
         * @brief 台座回避後に台座からの距離を調整する
         * @return 距離調整が終了したらtrue
         */
        bool run();
    };
};
#endif
