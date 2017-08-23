#include "RightAngledDetection.h"
#include "stdio.h"

namespace detection{
    RightAngledDetection::RightAngledDetection(){
        selfPos_ = measurement::SelfPositionEstimation::getInstance();
        color_ = device::ColorSensor::getInstance();
        counter_ = 0;

        for (int i = 0; i  < RAD_DATA_SIZE; i++){
            brightnessHistory_[i] = 0;
            distanceHistory_[i] = 0;
        }
    }

    bool RightAngledDetection::getResult(float minChangeRate){
        if (counter_ < RAD_DATA_SIZE)
            counter_++;

        /* 情報を更新する */
        for (int i = RAD_DATA_SIZE - 1;  0 < i ;  i--){
            brightnessHistory_[i] = brightnessHistory_[i - 1];
            distanceHistory_[i] = distanceHistory_[i - 1];
        }
        brightnessHistory_[0] = color_->getRelativeBrightness();
        distanceHistory_[0] = selfPos_->getMigrationLength();

       for (int start = 1;  start < counter_;  start++){
           int8_t  brightnessChanges = brightnessHistory_[0] - brightnessHistory_[start];
           long distanceChanges = distanceHistory_[0] - distanceHistory_[start];

           //距離の変化が小さすぎると誤検知が多いかもしれないので枝切り
           if (distanceChanges <= 3){
               continue;
           }

           //条件判定
           float changeRate = (float)brightnessChanges / (float)distanceChanges;
           if  (detectWhite_ &&  (changeRate >= minChangeRate) ){
                return true;
           }
           if  (detectBlack_ &&  (-changeRate >= minChangeRate) ){
                return true;
           }
       }
       return false;
    }

        void RightAngledDetection::setDetectBlack(bool detectBlack){
            detectBlack_ = detectBlack;
        }

        void RightAngledDetection::setDetectWhite(bool detectWhite){
            detectWhite_ = detectWhite;
        }
}
