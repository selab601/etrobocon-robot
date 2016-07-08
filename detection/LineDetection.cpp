#include "LineDetection.h"
#include "stdio.h"

namespace detection{
    LineDetection::LineDetection(){
        color_ = device::ColorSensor::getInstance();
        counter_ = 0;

        for (int i = 0; i  < LINE_DATA_SIZE; i++){
            brightnessHistory_[i] = 0;
        }
    }

    bool LineDetection::getResult(float minChangeRate){
        black_ = color_->getBlackCalibratedValue();
        if (counter_ < LINE_DATA_SIZE)
            counter_++;

        /* 情報を更新する */
        for (int i = LINE_DATA_SIZE - 1;  0 < i ;  i--){
            brightnessHistory_[i] = brightnessHistory_[i - 1];
        }

        brightnessHistory_[0] = color_->getBrightness();

       for (int start = 1;  start < counter_;  start++){
           int8_t  brightnessChanges = brightnessHistory_[0] - brightnessHistory_[start];
           brightnessChanges = brightnessChanges < 0
                                ? -brightnessChanges
                                : brightnessChanges;

           if  ( brightnessChanges >= minChangeRate
                 && brightnessHistory_[start] <= black_){
                return true;
           }
       }
       return false;
    }
}
