#include "ColorSensor.h"
using namespace std;

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
    {
        rgb_.r = rgb_.g = rgb_.b = 0;
        hsv_.h = hsv_.s = hsv_.v = 0;
    }


    int8_t ColorSensor::getBrightness(){
        return ev3api::ColorSensor::getBrightness();
    }

    double ColorSensor::getRelativeBrightness(bool useHsv){
        int diff = 0;   // 白と黒の差
        double relative = 0;
        if (useHsv){
            updateColor();
            diff = getVWhiteValue() - getVBlackValue();
            relative = getV() - getVBlackValue();
        }
        else{
            diff = getWhiteCalibratedValue() - getBlackCalibratedValue();
            relative = getBrightness() - getBlackCalibratedValue();
        }
        relative = relative * 100.0 / diff;
        return relative;
    }

    void ColorSensor::setCalibrateValue(int8_t whiteValue, int8_t blackValue){
        this->whiteCalibratedValue_ = whiteValue;
        this->blackCalibratedValue_ = blackValue;
    }

    void ColorSensor::setVValue(int whiteValue, int blackValue){
        this->vWhiteValue_ = whiteValue;
        this->vBlackValue_ = blackValue;
    }

    int8_t ColorSensor::getWhiteCalibratedValue(){
        return whiteCalibratedValue_;
    }

    int8_t ColorSensor::getBlackCalibratedValue(){
        return blackCalibratedValue_;
    }

    int ColorSensor::getVWhiteValue(){
        return vWhiteValue_;
    }
    int ColorSensor::getVBlackValue(){
        return vBlackValue_;
    }

    void ColorSensor::getRawColor(rgb_raw_t& rgb){
        ev3api::ColorSensor::getRawColor(rgb);
        rgb_ = rgb;
        hsv_ = rgb2Hsv(rgb_);
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

    void ColorSensor::updateColor(){
        ev3api::ColorSensor::getRawColor(rgb_);
        hsv_ = rgb2Hsv(rgb_);
    }

    int ColorSensor::getR(){return rgb_.r;}
    int ColorSensor::getG(){return rgb_.g;}
    int ColorSensor::getB(){return rgb_.b;}

    int ColorSensor::getH(){return hsv_.h;}
    int ColorSensor::getS(){return hsv_.s;}
    int ColorSensor::getV(){return hsv_.v;}

    hsv_raw_t ColorSensor::rgb2Hsv(rgb_raw_t rgbValue) {
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
