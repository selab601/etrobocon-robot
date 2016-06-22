/**
 * @file Color.h
 * @brief 輝度情報クラス
 * @author 兎澤 佑
 * @date 2015/07/02
 */

#ifndef BRIGHTNESS_INFO_
#define BRIGHTNESS_INFO_

#include "ColorSensor.h"

using namespace ev3api;

class Color
{
private:
    ColorSensor colorSensor;            //!< デバイスオブジェクト

    static Color* instance_;            //!< 共有インスタンス

    // キャリブレーション値
    int8_t white_calibrated_value_;     //!< 白基準値
    int8_t black_calibrated_value_;     //!< 黒基準値

private:
    Color();                            //!< コンストラクタ

public:
    /**
     * @brief インスタンスを取得する．
     * @details シングルトンパターンに基づいて，共有のインスタンスを取得する．
     * @return カラーセンサークラスの共有インスタンス
     * @author 兎澤 佑
     */
    static Color* getInstance();

    /**
     * @brief キャリブレーション値を設定する
     *
     * @param white_value 白基準値
     * @param brack_value 黒基準値
     * @author 兎澤 佑
     */
    void setCalibrateValue(int8_t white_value, int8_t black_value);

    /**
     * @brief キャリブレーション時に設定した白の基準値を取得する
     * @return 白の基準値
     * @author 兎澤 佑
     */
    int8_t getWhiteCalibratedValue();

    /**
     * @brief キャリブレーション時に設定した黒の基準値を取得する
     * @return 黒の基準値
     * @author 兎澤 佑
     */
    int8_t getBlackCalibratedValue();

    /**
     * @brief 輝度を取得する
     * @return 輝度
     * @author 兎澤 佑
     */
    int8_t getBrightness();

    /**
     * @brief 色を取得する
     * @return 色. @see http://ie.to/?71449186
     * @author 兎澤 佑
     */
    colorid_t getColor();
};

#endif
