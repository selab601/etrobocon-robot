/**
 * @file RightAngledDetection.h
 * @brief  直角検知クラス
 */
 #ifndef _RIGHT_ANGLED_DETECTION_H
 #define _RIGHT_ANGLED_DETECTION_H

 #include "../device/ColorSensor.h"
#include "../measurement/SelfPositionEstimation.h"
#include "../measurement/Coordinates.h"

#define RAD_DATA_SIZE 30    //RADはRightAngledDetectionの
 #define CHANGE_RATE 4.5

namespace detection{
    /**
     * @brief 直角検知クラス
     * @details 進んだ距離あたりのカラーセンサの値の変化量から直角検知を行う
     * ライントレースの速度を遅くする必要がある（ライントレースの速度補正50くらい)
     * @author motoki nagaoka
     */
    class RightAngledDetection {

    public:
        /**
         * @brief  コンストラクタ
          */
        RightAngledDetection();

        /**
         * @brief 直角を検知する
         * @details 内部状態更新もこのメソッドで同時に行っているので、検知したい場所の前から何回も呼び出していなければならない
         * 白→黒・黒→白のどちらでも判定します
         *
         * @return 直角を検知したとき: true, 検知していないとき: false
         */
        bool isDetected();

        bool isDetected(float changeRate);

    private:
        measurement::SelfPositionEstimation* selfPos;
        device::ColorSensor* color;

        /* カラーセンサの値*/
        int8_t brightnessHistory[RAD_DATA_SIZE];

        /* 進んだ距離 */
        long distanceHistory[RAD_DATA_SIZE];

        /* 現在のデータ数 */
        int counter;

      bool sendMessage(char* string);
    };

}
 #endif
