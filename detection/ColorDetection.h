#ifndef _COLOR_DETECTION_H
#define _COLOR_DETECTION_H

#include "../device/ColorSensor.h"
#include <vector>
#include <algorithm>
#include <ev3api.h>

namespace detection{

    class ColorDetection{
    private:
        device::ColorSensor* colorSensor_;
        colorid_t result_;
        colorid_t deltaResult_ = COLOR_NONE;
    public:
        ColorDetection();

        /**
         * @brief 検知した色を返す
         * @return 色
         */
        colorid_t getResult();

        /**
         * @brief 赤青黃緑の四色かどうかを判定する
         * @return 赤青黃緑:true,それ以外:false
         */
        bool isFourColors();

    private:

        colorid_t confirmedColor();
    };
}

#endif
