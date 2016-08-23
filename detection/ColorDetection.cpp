#include "ColorDetection.h"

using namespace device;
using namespace std;

namespace detection{
  ColorDetection::ColorDetection(){
    colorSensor_ = ColorSensor::getInstance();
  }

    colorid_t ColorDetection::getResult(){
        colorSensor_->getRawColor(rgbColor_);
        greenPerRed_ = rgbColor_.g*(1.0)/(rgbColor_.r + 0.01);  //! r値とg値の比
        bluePerRed_ = rgbColor_.b*(1.0)/(rgbColor_.r + 0.01);   //! r値とb値の比

        hsv_raw_t hsv = detection::ColorDetection::Rgb2Hsv(rgbColor_);

        colorid_t result;
        if (hsv.s <= 160) {
            /* 彩度が低い場合はモノクロと判断する */
            /* モノクロの場合，明度から白か黒かを判断する */
            if (hsv.v < 30) {
                result = COLOR_BLACK;
            } else if (hsv.v > 150) {
                result = COLOR_WHITE;
            } else {
                result = COLOR_NONE;
            }
        } else {
            /* 彩度が高い場合は色があると判断する */
            /* 色がある場合，色相からその色を判断する */
            if (hsv.h >= 0 && hsv.h <= 10) {
                result = COLOR_RED;
            } else if (hsv.h >= 30 && hsv.h <= 60) {
                result = COLOR_YELLOW;
            } else if (hsv.h >= 110 && hsv.h <= 140) {
                result = COLOR_GREEN;
            } else if (hsv.h >= 170 && hsv.h <= 220) {
                result = COLOR_BLUE;
            } else {
                result = COLOR_NONE;
            }
        }

        return result;
    }

    hsv_raw_t ColorDetection::Rgb2Hsv(rgb_raw_t rgbValue) {
        vector<float> rgb {
            (float)rgbValue.r,
            (float)rgbValue.g,
            (float)rgbValue.b
        };

        int iMaxValue = 0;
        int iMinValue = 0;
        float maxValue = 0.;
        float minValue = 255.;
        for(auto value = begin(rgb); value != end(rgb); ++value) {
            if (*value > maxValue) {
                maxValue = *value;
                iMaxValue = distance(rgb.begin(), value);

                // 255 以上の値は切り捨てる
                if (maxValue > 255) {
                    maxValue = 255;
                    rgb[iMaxValue] = 255;
                }
            }
            if (*value < minValue) {
                minValue = *value;
                iMinValue = distance(rgb.begin(), value);
            }
        }

        int h;
        if (iMaxValue == iMinValue) {
            h = 0;
        } else if (iMaxValue == 0 /* R値が最も高い */) {
            h = (int)(60*(rgb.at(1)-rgb.at(2))/(maxValue-minValue)+360)%360;
        } else if (iMaxValue == 1 /* G値が最も高い */) {
            h = (int)(60*(rgb.at(2)-rgb.at(0))/(maxValue-minValue))+120;
        } else if (iMaxValue == 2 /* B値が最も高い */) {
            h = (int)(60*(rgb.at(0)-rgb.at(1))/(maxValue-minValue))+240;
        } else {
            // WARNING: ここにはこないはず
            h = 0;
        }

        int s;
        if (maxValue == 0) {
            s = 0;
        } else {
            s = (255*((maxValue - minValue)/maxValue));
        }

        int v = maxValue;

        hsv_raw_t hsv = {h,s,v};
        return hsv;
    }
}
