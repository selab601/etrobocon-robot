#ifndef COLORSENSOR_H_
#define COLORSENSOR_H_

#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/ColorSensor.h"

namespace device
{
    class ColorSensor : public ev3api::ColorSensor
    {
    private:
        static ColorSensor* instance_;

        // キャリブレーション値
        //! 白基準値
        int8_t whiteCalibratedValue_;
        //! 黒基準値
        int8_t blackCalibratedValue_;

    private:
        ColorSensor();

    public:
        static ColorSensor* getInstance();

        void setCalibrateValue(int8_t whiteValue, int8_t blackValue);

        int8_t getWhiteCalibratedValue();

        int8_t getBlackCalibratedValue();
    };
}

#endif
