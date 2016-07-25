#include "LineDetection.h"
#include "stdio.h"

namespace detection{
    LineDetection::LineDetection(){
        Initialize();
    }

    bool LineDetection::getResult(float changeRate){
        Initialize();
        black_ = color_->getBlackCalibratedValue();
        white_ = color_->getWhiteCalibratedValue();
        int8_t diff_ = white_ - black_;

        if(counter_ < LINE_DATA_SIZE){
            counter_++;
        }

        /* 情報を更新する */
        for(int i = LINE_DATA_SIZE - 1;  0 < i ;  i--){
            brightnessHistory_[i] = brightnessHistory_[i - 1];
        }

        brightnessHistory_[0] = color_->getBrightness();

        for(int start = 1;  start < counter_;  start++){
            int8_t  brightnessChanges = brightnessHistory_[0] - brightnessHistory_[start];
            brightnessChanges = brightnessChanges < 0
                                ? -brightnessChanges
                                : brightnessChanges;

            if(brightnessChanges <= changeRate){
                continue;
            }
            if(brightnessChanges <= diff_){
                return true;
            }
        }
        return false;
    }

    void LineDetection::Initialize(){
        color_ = device::ColorSensor::getInstance();
        counter_ = 0;

        for (int i = 0; i  < LINE_DATA_SIZE; i++){
            brightnessHistory_[i] = 0;
        }
    }
}
