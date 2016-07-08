/**
 * @file ObjectDetection.cpp
 * @brief 障害物検知クラス
 */
#include "ObjectDetection.h"

using namespace ev3api;

namespace detection{
    /* コンストラクタ */
    ObjectDetection::ObjectDetection(u_int Distance){
        sonar_ = device::SonarSensor::getInstance();
        u_int detectDistance_ = Distance;
    }

    bool ObjectDetection::getResult(){
        if(u_int(sonar_->getDistance()) < detectDistance_){
        	return true;
        }
        return false;
    }
};
