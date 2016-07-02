/**
 * @file ObjectDetection.h
 * @brief 障害物検知クラス
 */

#ifndef _OBJECT_DETECTION_H
#define _OBJECT_DETECTION_H

#include "ev3api.h"
#include "../device/Display.h"
#include "../device/SonarSensor.h"


namespace detection
{
    class ObjectDetection
    {
    private:
        device::SonarSensor* sonar_;
        long detect_distance_;

    public:
        /**
         * @brief コンストラクタ
         */
        ObjectDetection();

        /**
         * @brief 障害物検知
         * @return 検知したらreturnを返す
         */
        bool isDetected();

        /**
         * @brief 検知する距離の設定
         * 単位はcm
         */
        void setDetectDistance(long distance);
    };
};

#endif
