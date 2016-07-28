/**
 * @file LineDetection.h
 * @brief  ライン検知クラス
 */
#ifndef _LINE_DETECTION_H
#define _LINE_DETECTION_H

#include "../device/ColorSensor.h"

#define LINE_DATA_SIZE 30
#define LINE_CHANGE_RATE 20     //緑とか検知しそうなので枝切り用

namespace detection{
    /**
     * @brief ライン検知クラス
     * @details 進んだ距離あたりのカラーセンサの値の輝度変化量からライン検知を行う
     */
    class LineDetection{
        private:
            device::ColorSensor* color_;
            //カラーセンサクラスが保持しているキャリブレーション値と黒白の差
            int8_t black_;
            int8_t white_;
            int8_t diff_;

            /* カラーセンサの値*/
            int8_t brightnessHistory_[LINE_DATA_SIZE];
            /* 現在のデータ数 */
            int counter_;

        private:
            /**
             * @brief 初期化
             */
            void Initialize();

        public:
            /**
             * @brief  コンストラクタ
              */
            LineDetection();

            /**
             * @brief ラインを検知する
             * @details 内部状態更新もこのメソッドで同時に行っているので、検知したい場所の前から何回も呼び出していなければならない
             * @param changeRate ライン検知の精度を変える変数．与えられなければデフォルト値を与えて実行．
             *        changeRateを低くすると検知の精度が高く（甘く）なる．低すぎると影の変化とか多少の変化でも検知してしまうかもしれない．
             *        MAX:白のキャリブレーション値から黒のキャリブレーション値を引いた値（例:74-6=68)
             *        min:0
             * @return ラインを検知したとき: true, 検知していないとき: false
             */
            bool getResult(float changeRate = LINE_CHANGE_RATE);

    };
}
 #endif
