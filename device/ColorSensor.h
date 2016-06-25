#ifndef COLORSENSOR_H_
#define COLORSENSOR_H_

#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/ColorSensor.h"

namespace device
{
    class ColorSensor : ev3api::ColorSensor
    {
    private:
        static ColorSensor* instance_;

        // キャリブレーション値
        //! 白基準値
        int8_t white_calibrated_value_;
        //! 黒基準値
        int8_t black_calibrated_value_;

    private:
        ColorSensor();

    public:
        static ColorSensor* getInstance();

        void setCalibrateValue(int8_t white_value, int8_t black_value);

        int8_t getWhiteCalibratedValue();

        int8_t getBlackCalibratedValue();
    };
}

#endif
