/**
 * @file ObjectDetection.h
 * @brief 障害物検知クラス
 */

#ifndef _OBJECT_DETECTION_H
#define _OBJECT_DETECTION_H

#include "ev3api.h"
#include "../device/SonarSensor.h"

namespace detection
{
    class ObjectDetection
    {
    private:
        device::SonarSensor* sonar_;
        u_int detectDistance_;

    public:
        /**
         * @brief コンストラクタ
         * @param Distance 検知する距離.単位はcm
         */
        ObjectDetection(u_int Distance);

        /**
         * @brief 障害物検知
         * @return 検知したらtrueを返す
         */
        bool getResult();
    };
};

#endif
