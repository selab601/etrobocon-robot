/**
 * @file ObjectDetection.cpp
 * @brief 障害物検知クラス
 */
#include "ObjectDetection.h"

namespace detection {
    /* コンストラクタ */
    ObjectDetection::ObjectDetection(u_int distance) {
        sonar_ = device::SonarSensor::getInstance();
        detectDistance_ = distance;
    }

    bool ObjectDetection::getResult() {
        if(u_int(sonar_->getDistance()) < detectDistance_) {
        	return true;
        }
    return false;
    }
};
