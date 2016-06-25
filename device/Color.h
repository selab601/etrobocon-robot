#ifndef COLORSENSOR_H_
#define COLORSENSOR_H_

#include "ColorSensor.h"

using namespace ev3api;

class Color
{
private:
    //! デバイスオブジェクト
    ColorSensor colorSensor;

    //! 共有インスタンス
    static Color* instance_;

    // キャリブレーション値
    //! 白基準値
    int8_t white_calibrated_value_;
    //! 黒基準値
    int8_t black_calibrated_value_;

private:
    Color();

public:
    static Color* getInstance();

    void setCalibrateValue(int8_t white_value, int8_t black_value);

    int8_t getWhiteCalibratedValue();

    int8_t getBlackCalibratedValue();

    /**
     * @brief 反射光の強さを取得する
     * @return 反射光の強さ (0-100)
     */
    int8_t getBrightness();

    /**
     * @brief 環境光の強さを取得する
     * @return 環境光の強さ (0-100)
     */
    uint8_t getAmbient();

    /**
     * @brief 色を取得する
     * @return 色
     * @see http://www.toppers.jp/ev3pf/EV3RT_C_API_Reference/group__ev3sensor.html#gaf11750614f023e665f98eca0b1f79c2f
     */
    colorid_t getColor();

    /**
     * @brief RGB Raw値を測定する
     * @param rgb 取得した値を格納する変数のポインタ
     */
    void getRawColor(rgb_raw_t &rgb);
};

#endif
