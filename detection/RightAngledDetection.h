/**
 * @file RightAngledDetection.h
 * @brief  直角検知クラス
 */
#ifndef _RIGHT_ANGLED_DETECTION_H
#define _RIGHT_ANGLED_DETECTION_H

#include "../device/ColorSensor.h"
#include "../device/Display.h"
#include "../measurement/SelfPositionEstimation.h"

#define RAD_DATA_SIZE 30    //RADはRightAngledDetectionの略
#define CHANGE_RATE 0.5

namespace detection{
    /**
     * @brief 直角検知クラス
     * @details 進んだ距離あたりのカラーセンサの値の変化量から直角検知を行う
     * ライントレースの速度を遅くする必要がある（ライントレースの速度補正50くらい)
     */
    class RightAngledDetection
    {
    private:
        measurement::SelfPositionEstimation* selfPos_;
        device::ColorSensor* color_;
        device::Display* disp_;

        /* カラーセンサの値*/
        int8_t brightnessHistory[RAD_DATA_SIZE];
        /* 進んだ距離 */
        long distanceHistory[RAD_DATA_SIZE];
        /* 現在のデータ数 */
        int counter_;

    public:
        /**
         * @brief  コンストラクタ
          */
        RightAngledDetection();

        /**
         * @brief 直角を検知する
         * @details 内部状態更新もこのメソッドで同時に行っているので、検知したい場所の前から何回も呼び出していなければならない
         * 白→黒・黒→白のどちらでも判定します
         * @return 直角を検知したとき: true, 検知していないとき: false
         */
        bool getResult();

        bool getResult(float changeRate);
    };
}
 #endif
