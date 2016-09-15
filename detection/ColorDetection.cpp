#include "ColorDetection.h"

using namespace device;
using namespace std;

namespace detection{
    ColorDetection::ColorDetection(){
        colorSensor_ = ColorSensor::getInstance();
    }

    colorid_t ColorDetection::getResult(){
        colorSensor_->getRawColor(rgbColor_);

        hsv_raw_t hsv = detection::ColorDetection::Rgb2Hsv(rgbColor_);

        colorid_t result;
        // これ以下の各閾値は，実験から求めた．詳しくは2016年度のモデル参照
        if (hsv.v < 40) {
            result = COLOR_BLACK;
        } else if (hsv.s <= 165) {
            /* 彩度が低い場合はモノクロと判断する */
            /* モノクロの場合，明度から白か黒かを判断する */
            if (hsv.v > 30) {
                result = COLOR_WHITE;
            } else {
                result = COLOR_NONE;
            }
        } else {
            /* 彩度が高い場合は色があると判断する */
            /* 色がある場合，色相からその色を判断する */
            if (hsv.h >= 0 && hsv.h <= 29) {
                result = COLOR_RED;
            } else if (hsv.h >= 30 && hsv.h <= 80) {
                result = COLOR_YELLOW;
            } else if (hsv.h >= 110 && hsv.h <= 140) {
                result = COLOR_GREEN;
            } else if (hsv.h >= 150 && hsv.h <= 220) {
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

        // 255より大きいRGB値は255に合わせる
        for_each(rgb.begin(), rgb.end(), [](float &v){ if (v > 255) { v = 255; } });

        vector<float>::iterator minPtr = min_element(rgb.begin(), rgb.end());
        vector<float>::iterator maxPtr = max_element(rgb.begin(), rgb.end());
        // 接頭辞 i は，index を意味する．
        // iMaxValue は maxValue の index．iMinValue も同様
        int iMaxValue = (int)distance(rgb.begin(), maxPtr);
        int iMinValue = (int)distance(rgb.begin(), minPtr);
        float maxValue = *maxPtr;
        float minValue = *minPtr;

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
