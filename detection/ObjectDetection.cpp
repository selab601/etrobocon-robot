/**
 * @file ObjectDetection.cpp
 * @brief 障害物検知クラス
 */

#include "ObjectDetection.h"
#include "ev3api.h"
#include "../device/display.h"
#include "../device/SonarSensor.h"


namespace detection
{
    //コンストラクタ
    ObjectDetection::ObjectDetection(){
        display_   = Display::getInstance();
        sonar_       = SonarSensor::getInstance();
        detect_distance_    = 0;
    }

    bool ObjectDetection::isDetected(){
        if(sonar_ -> getSonarValue() <= detect_distance_){
        	display_->updateDisplay("Object is detected !",  7);
        	return true;
        }

        return false;
    }

    void ObjectDetection::setDetectDistance(long distance){
        detect_distance_ = distance;
    }
};
