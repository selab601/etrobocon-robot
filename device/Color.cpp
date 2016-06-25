#include "Color.h"

// インスタンス変数初期化
Color* Color::instance_ = 0;

// シングルトン
Color* Color::getInstance(){
    if (!instance_) {
        instance_ = new Color();
    }
    return instance_;
}

Color::Color() :
    // ポートの指定
    ColorSensor(PORT_3),
    white_calibrated_value_(0),
    black_calibrated_value_(0)
{}

void Color::setCalibrateValue(int8_t white_value, int8_t black_value){
    this->white_calibrated_value_ = white_value;
    this->black_calibrated_value_ = black_value;
}

int8_t Color::getWhiteCalibratedValue(){
    return white_calibrated_value_;
}

int8_t Color::getBlackCalibratedValue(){
    return black_calibrated_value_;
}
