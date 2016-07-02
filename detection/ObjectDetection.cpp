/**
 * @file ObjectDetection.cpp
 * @brief 障害物検知クラス
 */
#include "ObjectDetection.h"

using namespace ev3api;

namespace detection{
    /* コンストラクタ */
    ObjectDetection::ObjectDetection(){
        sonar_   = device::SonarSensor::getInstance();
        detect_distance_    = 0;
    }

    bool ObjectDetection::isDetected(){
        if(sonar_->getDistance() <= detect_distance_){
        	return true;
        }
        return false;
    }
    void ObjectDetection::setDetectDistance(long distance){
        detect_distance_ = distance;
    }
};
