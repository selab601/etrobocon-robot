#ifndef COLORSENSOR_H_
#define COLORSENSOR_H_

#include "ColorSensor.h"

using namespace ev3api;

class Color : ev3api::ColorSensor
{
private:
    static Color* instance_;

    // キャリブレーション値
    //! 白基準値
    int8_t white_calibrated_value_;
    //! 黒基準値
    int8_t black_calibrated_value_;

private:
    Color();

public:
    static Color* getInstance();

    void setCalibrateValue(int8_t white_value, int8_t black_value);

    int8_t getWhiteCalibratedValue();

    int8_t getBlackCalibratedValue();
};

#endif
