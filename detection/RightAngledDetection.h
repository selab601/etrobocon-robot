/**
 * @file RightAngledDetection.h
 * @brief  直角検知クラス
 */
#ifndef _RIGHT_ANGLED_DETECTION_H
#define _RIGHT_ANGLED_DETECTION_H

#include "../device/ColorSensor.h"
#include "../measurement/SelfPositionEstimation.h"

#define RAD_DATA_SIZE 30    //RADはRightAngledDetectionの略
#define RAD_CHANGE_RATE 5.0

namespace detection{
    /**
     * @brief 直角検知クラス
     * @details 進んだ距離あたりのカラーセンサの値の変化量から直角検知を行う
     * ライントレースの速度を遅くする必要がある(pwm20以下で)
     */
    class RightAngledDetection
    {
    private:
        measurement::SelfPositionEstimation* selfPos_;
        device::ColorSensor* color_;

        /* カラーセンサの値*/
        int8_t brightnessHistory_[RAD_DATA_SIZE];
        /* 進んだ距離 */
        long distanceHistory_[RAD_DATA_SIZE];
        /* 現在のデータ数 */
        int counter_;

        //! 黒の直角を判定するか
        bool detectBlack_ = true;
        //! 白の直角を判定するか
        bool detectWhite_ = true;

    public:
        /**
         * @brief  コンストラクタ
          */
        RightAngledDetection();

        /**
         * @brief 直角を検知する
         * @details 内部状態更新もこのメソッドで同時に行っているので、検知したい場所の前から何回も呼び出していなければならない
         * 白→黒・黒→白のどちらでも判定します
         * @param changeRate 直角検知の精度を変える変数．与えられなければデフォルト値を与えて実行．
         * @return 直角を検知したとき: true, 検知していないとき: false
         */
        bool getResult(float changeRate = RAD_CHANGE_RATE);


        /**
         * @brief 白->黒の直角を判定するか指定する
         *
         * @param detectBlack 判定するか
         */
        void setDetectBlack(bool detectBlack);

        /**
         * @brief 黒->白の直角を判定するか指定する
         *
         * @param detectWhite 判定するか
         */
        void setDetectWhite(bool detectWhite);
    };
}
 #endif
