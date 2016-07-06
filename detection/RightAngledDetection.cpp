#include "RightAngledDetection.h"
#include "stdio.h"


namespace detection{
    RightAngledDetection::RightAngledDetection(){
        selfPos_ = measurement::SelfPositionEstimation::getInstance();
        color_ = device::ColorSensor::getInstance();
        disp_ = device::Display::getInstance();
        counter_ = 0;

        for (int i = 0; i  < RAD_DATA_SIZE; i++){
            brightnessHistory[i] = 0;
            distanceHistory[i] = 0;
        }

    }

    bool RightAngledDetection::getResult(){
        return getResult(CHANGE_RATE);
    }

    bool RightAngledDetection::getResult(float minChangeRate){
        if (counter_ < RAD_DATA_SIZE)
            counter_++;

        /* 情報を更新する */
        for (int i = RAD_DATA_SIZE - 1;  0 < i ;  i--){
            brightnessHistory[i] = brightnessHistory[i - 1];
            distanceHistory[i] = distanceHistory[i - 1];
        }
        brightnessHistory[0] = color_->getBrightness();
        distanceHistory[0] = selfPos_->getMigrationLength();

       for (int start = 1;  start < counter_;  start++){
           disp_->updateDisplay("now detecting...",12);
           int8_t  brightnessChanges = brightnessHistory[0] - brightnessHistory[start];
           brightnessChanges = brightnessChanges < 0? -brightnessChanges: brightnessChanges;
           long distanceChanges = distanceHistory[0] - distanceHistory[start];

           //距離の変化が小さすぎると誤検知が多いかもしれないので枝切り
           //if (distanceChanges <= 3) continue;

            //条件判定
           float changeRate = (float)brightnessChanges / (float)distanceChanges;
           if  ( changeRate >= minChangeRate ){
               disp_->updateDisplay("detected!",12);
                return true;
           }
       }
       return false;
    }
}
