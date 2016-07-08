/**
 * @file LineDetection.h
 * @brief  ライン検知クラス
 */
#ifndef _LINE_DETECTION_H
#define _LINE_DETECTION_H

#include "../device/ColorSensor.h"
//#include "../measurement/SelfPosLINEonEstimation.h"

#define LINE_DATA_SIZE 30    //RADはRightAngledDetectionの略
#define CHANGE_RATE 70

namespace detection {
    /**
     * @brief ライン検知クラス
     * @details 進んだ距離あたりのカラーセンサの値の変化量からライン検知を行う
     * 速度を遅くする必要がある(pwm20以下で)
     */
    class LineDetection {
        private:
            //measurement::SelfPositionEstimation* selfPos_;
            device::ColorSensor* color_;
            int8_t black_;
            /* カラーセンサの値*/
            int8_t brightnessHistory_[LINE_DATA_SIZE];
            /* 現在のデータ数 */
            int counter_;

        public:
            /**
             * @brief  コンストラクタ
              */
            LineDetection();

            /**
             * @brief ラインを検知する
             * @details 内部状態更新もこのメソッドで同時に行っているので、検知したい場所の前から何回も呼び出していなければならない
             * @param changeRate ライン検知の精度を変える変数．与えられなければデフォルト値を与えて実行．
             * @return ラインを検知したとき: true, 検知していないとき: false
             */
            bool getResult(float changeRate = CHANGE_RATE);
    };
}
 #endif
