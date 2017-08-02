#include "ColorDetection.h"

using namespace device;
using namespace std;

namespace detection{
    ColorDetection::ColorDetection(){
        colorSensor_ = ColorSensor::getInstance();
    }

    colorid_t ColorDetection::getResult(){
        colorSensor_->updateColor(); // RGB,HSVを更新


        colorid_t result;
        // これ以下の各閾値は，実験から求めた．詳しくは2016年度のモデル参照
        if (colorSensor_->getV() < 30) {
            result = COLOR_BLACK;
        } else if (colorSensor_->getS() <= 160) {
            /* ここで板の色を切ってしまうと青や緑に影響する */
            /* 彩度が低い場合はモノクロと判断する */
            /* モノクロの場合，明度から白か黒かを判断する */
            if (colorSensor_->getV() > 30) {
                result = COLOR_WHITE;
            } else {
                result = COLOR_NONE;
            }
        } else {
            /* 彩度が高い場合は色があると判断する */
            /* 色がある場合，色相からその色を判断する */
            if (colorSensor_->getH() >= 0 && colorSensor_->getH() <= 30) {
                result = COLOR_RED;
            } else if (colorSensor_->getH() >= 40 && colorSensor_->getH() <= 80) {
                /* 彩度が高ければ黄色と判断する
                   彩度が低ければ板の色(白)と判断する*/
                if(colorSensor_->getS() >= 190){
                    result = COLOR_YELLOW;
                }else{
                    result = COLOR_WHITE;
                }
            } else if (colorSensor_->getH() >= 90 && colorSensor_->getH() <= 130) {
                result = COLOR_GREEN;
            } else if (colorSensor_->getH() >= 135 && colorSensor_->getH() <= 220) {
                result = COLOR_BLUE;
            } else {
                result = COLOR_NONE;
            }
        }

        return result;
    }

    bool ColorDetection::isFourColors(){
        colorid_t nowColor;
        nowColor = getResult();
        if(nowColor == COLOR_BLUE ||
            nowColor == COLOR_RED ||
            nowColor == COLOR_GREEN ||
            nowColor == COLOR_YELLOW){
            return true;
        }else {
            return false;
        }
    }
}
