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
        whiteCalibratedValue_(0),
        blackCalibratedValue_(0)
    {}

    void ColorSensor::setCalibrateValue(int8_t whiteValue, int8_t blackValue){
        this->whiteCalibratedValue_ = whiteValue;
        this->blackCalibratedValue_ = blackValue;
    }

    int8_t ColorSensor::getWhiteCalibratedValue(){
        return whiteCalibratedValue_;
    }

    int8_t ColorSensor::getBlackCalibratedValue(){
        return blackCalibratedValue_;
    }

    const char* ColorSensor::getColorName(colorid_t id) {
        switch (id) {
        case COLOR_NONE:
            return "NONE";
        case COLOR_RED:
            return "RED";
        case COLOR_BLUE:
            return "BLUE";
        case COLOR_GREEN:
            return "GREEN";
        case COLOR_BLACK:
            return "BLACK";
        case COLOR_YELLOW:
            return "YELLOW";
        default:
            return "UNDEFINED";
        }
    }
}
