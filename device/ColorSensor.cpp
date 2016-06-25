#include "ColorSensor.h"

namespace device
{
    // インスタンス変数初期化
    ColorSensor* ColorSensor::instance_ = 0;

    // シングルトン
    ColorSensor* ColorSensor::getInstance(){
        if (!instance_) {
            instance_ = new ColorSensor();
        }
        return instance_;
    }

    ColorSensor::ColorSensor() :
        // ポートの指定
        ev3api::ColorSensor(PORT_3),
        white_calibrated_value_(0),
        black_calibrated_value_(0)
    {}

    void ColorSensor::setCalibrateValue(int8_t white_value, int8_t black_value){
        this->white_calibrated_value_ = white_value;
        this->black_calibrated_value_ = black_value;
    }

    int8_t ColorSensor::getWhiteCalibratedValue(){
        return white_calibrated_value_;
    }

    int8_t ColorSensor::getBlackCalibratedValue(){
        return black_calibrated_value_;
    }
}
