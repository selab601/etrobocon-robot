#ifndef COLORSENSOR_H_
#define COLORSENSOR_H_

#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/ColorSensor.h"
#include <algorithm>
#include <vector>

namespace device
{
    // rgb_raw_t に合わせた記法を採用(スネークケース)
    typedef struct hsv_raw {
        int h;
        int s;
        int v;
    } hsv_raw_t;

    class ColorSensor : public ev3api::ColorSensor
    {
    private:
        static ColorSensor* instance_;

        // キャリブレーション値
        //! 白基準値
        int8_t whiteCalibratedValue_;
        //! 黒基準値
        int8_t blackCalibratedValue_;

        rgb_raw_t rgb_;
        hsv_raw_t hsv_;


    private:
        ColorSensor();

    public:
        static ColorSensor* getInstance();

        void setCalibrateValue(int8_t whiteValue, int8_t blackValue);

        int8_t getWhiteCalibratedValue();

        int8_t getBlackCalibratedValue();

        /**
         * @brief (Override) RGB Raw値を測定する
         *
         * @param rgb 取得した値を格納する変数のポインタ
         */
        void getRawColor(rgb_raw_t& rgb);

        /**
         * @brief RBGのR値を取得する
         * @details updateColor()で更新した時の値を取得する(これを呼ぶ前にupdateColor() を呼んでおく)
         *
         * @return R値
         */
        int getR();
        /**
         * @brief RBGのG値を取得する
         * @details updateColor()で更新した時の値を取得する(これを呼ぶ前にupdateColor() を呼んでおく)
         *
         * @return G値
         */
        int getG();
        /**
         * @brief RBGのB値を取得する
         * @details updateColor()で更新した時の値を取得する(これを呼ぶ前にupdateColor() を呼んでおく)
         *
         * @return B値
         */
        int getB();

        /**
         * @brief HSVのH値を取得する
         * @details updateColor()で更新した時の値を取得する(これを呼ぶ前にupdateColor() を呼んでおく)
         *
         * @return H値
         */
        int getH();
        /**
         * @brief HSVのS値を取得する
         * @details updateColor()で更新した時の値を取得する(これを呼ぶ前にupdateColor() を呼んでおく)
         *
         * @return S値
         */
        int getS();
        /**
         * @brief HSVのV値を取得する
         * @details updateColor()で更新した時の値を取得する(これを呼ぶ前にupdateColor() を呼んでおく)
         *
         * @return V値
         */
        int getV();

        /**
         * @brief 内部のRGB,HSVの値を更新する
         * @details getR()等を呼ぶ前にこれを呼んでおく
         */
        void updateColor();

        static const char* getColorName(colorid_t id);

        /**
         * @brief RGB値をHSV値に変換する
         * @return HSV値
         */
        static hsv_raw_t rgb2Hsv(rgb_raw_t rgbValue);
    };
}

#endif
