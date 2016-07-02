/**
 * @file TrainDetection.h
 * @brief 新幹線検知クラス
 * @author sisido
 */

#ifndef _TRAIN_DETECTION_H
#define _TRAIN_DETECTION_H

#include "IDetection.h"
#include "ev3api.h"
#include "../device/display.h"
#include "../device/Sonar.h"


namespace detection{

    class TrainDetection : public IDetection {
    private:
        /* 目標とする対象までの測定距離 */
        long  detect_distance_;

        Display* display_;
        Sonar* sonar_;

    public:
        /**
         * @brief コンストラクタ
         * @author sisido
         */
        TrainDetection();


        /**
         * @brief 検知クラス．
         * ソナーセンサで検知した対象までの距離が検知距離以内なら音が鳴ります。
         * lcdモニタにも検出していることを表示します。
         * @return 目標距離以内に対象物がある{true}, ない{false}
         * @author sisido
         */
        bool isDetected();

        /**
         * @brief ソナーで検知する対象までの距離を設定する
         * 単位は[cm]です
         * @author sisido
         */
        void setDetectDistance(long distance);
    };
};

#endif
