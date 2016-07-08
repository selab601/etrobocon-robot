/**
 * @file LineDetection.h
 * @brief  ライン検知クラス
 */
#ifndef _LINE_DETECTION_H
#define _LINE_DETECTION_H

#include "../device/ColorSensor.h"
//#include "../measurement/SelfPosLINEonEstimation.h"

#define LINE_DATA_SIZE 30
#define CHANGE_RATE 20     //緑とか検知しそうなので枝切り用

namespace detection {
    /**
     * @brief ライン検知クラス
     * @details 進んだ距離あたりのカラーセンサの値の変化量からライン検知を行う
     */
    class LineDetection {
        private:
            //measurement::SelfPositionEstimation* selfPos_;
            device::ColorSensor* color_;
            int8_t black_;
            int8_t white_;
            int8_t diff_;

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
