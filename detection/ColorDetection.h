#ifndef _COLOR_DETECTION_H
#define _COLOR_DETECTION_H

#include "../device/ColorSensor.h"
#include <vector>
#include <algorithm>
#include <ev3api.h>

namespace detection{
    // rgb_raw_t に合わせた記法を採用(スネークケース)
    typedef struct hsv_raw {
        int h;
        int s;
        int v;
    } hsv_raw_t;

    class ColorDetection{
    private:
        rgb_raw_t rgbColor_;             //! rgb値
        device::ColorSensor* colorSensor_;
    public:
        ColorDetection();

        /**
         * @brief 検知した色を返す
         * @return 色
         */
        colorid_t getResult();

        /**
         * @brief RGB値をHSV値に変換する
         * @return HSV値
         */
        static hsv_raw_t Rgb2Hsv(rgb_raw_t rgbValue);
    };
}

#endif
