/**
 * @file ObjectDetection.cpp
 * @brief 障害物検知クラス
 */
#include "ObjectDetection.h"

using namespace ev3api;

namespace detection{
    /* コンストラクタ */
    ObjectDetection::ObjectDetection(u_int Distance){
        sonar = device::SonarSensor::getInstance();
        u_int detectDistance = Distance;
    }

    bool ObjectDetection::getResult(){
        if(u_int(sonar->getDistance()) < detectDistance){
        	return true;
        }
        return false;
    }
};
