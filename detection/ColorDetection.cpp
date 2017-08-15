#include "ColorDetection.h"

using namespace device;
using namespace std;

namespace detection{
    ColorDetection::ColorDetection(){
        colorSensor_ = ColorSensor::getInstance();
    }

    colorid_t ColorDetection::getResult(){
        colorSensor_->updateColor(); // RGB,HSVを更新


        // これ以下の各閾値は，実験から求めた．詳しくは2016年度のモデル参照
        if (colorSensor_->getV() < 30) {
            result_ = COLOR_BLACK;
        } else if (colorSensor_->getS() <= 160) {
            /* ここで板の色を切ってしまうと青や緑に影響する */
            /* 彩度が低い場合はモノクロと判断する */
            /* モノクロの場合，明度から白か黒かを判断する */

            //2017年は紙の4色があり、夜の紙の青が[H:141,S:145,V:65]周辺となる
            //青に合わせてS値の条件を緩めてしまうと白→黒のときに一瞬143付近まで上昇するため
            //その時のH値が黄色付近となり黄色と判定してしまう
            //ライントレース中にH値が青周辺になることはないので先に青を判定する
            if (colorSensor_->getH() >= 135 && colorSensor_->getH() <= 220){
                result_ = COLOR_BLUE;
            }else if (colorSensor_->getV() > 30) {
                result_ = COLOR_WHITE;
            } else {
                result_ = COLOR_NONE;
            }
        } else {
            /* 彩度が高い場合は色があると判断する */
            /* 色がある場合，色相からその色を判断する */
            if (colorSensor_->getH() >= 0 && colorSensor_->getH() <= 30) {
                result_ = COLOR_RED;
            } else if (colorSensor_->getH() >= 40 && colorSensor_->getH() <= 80) {
                /* 彩度が高ければ黄色と判断する
                   彩度が低ければ板の色(白)と判断する*/
                if(colorSensor_->getV() <= 160){
                    result_ = COLOR_BLACK;
                }else if(colorSensor_->getS() >= 190){
                    result_ = COLOR_YELLOW;
                }else{
                    result_ = COLOR_WHITE;
                }
            } else if (colorSensor_->getH() >= 90 && colorSensor_->getH() <= 130) {
                result_ = COLOR_GREEN;
            } else if (colorSensor_->getH() >= 135 && colorSensor_->getH() <= 220) {
                result_ = COLOR_BLUE;
            } else {
                result_ = COLOR_NONE;
            }
        }

        return confirmedColor();
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

    colorid_t ColorDetection::confirmedColor(){
        if(result_ == COLOR_BLUE){//青のH値に到達するまで時間がかかるので例外処理
            return result_;
        }
        if(deltaResult_ == result_){//前回(4ms前)と同じでないと色として確定しない
            return result_;
        }else{
            deltaResult_ = result_;
            return COLOR_NONE;
        }
    }
}
