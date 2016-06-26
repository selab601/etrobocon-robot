/**
 * @file Starter.h
 * @brief  スタータクラス
 * @author motoki nagaoka unko
 * @date 2015/07/15
 */

#ifndef STARTER_
#define STARTER_

#include "Starter.h"
#include "../device/TouchSensor.h"
#include "../device/Display.h"


namespace contest{
    /**
     * @brief スタータクラス
     * @details タッチセンサからスタートの判定を行う
     */
    class Starter{
        public:
            /**
             * @brief スタートを受け付ける
             * @details タッチセンサが押されるまでまつ
             * @return タッチセンサが押されたらtrue
             */
            bool start();           // スタートを受け付ける

            /**
        * @brief インスタンスを取得する．
        * @details シングルトンパターンに基づいて，共有のインスタンスを取得する．
        * @return スタータクラスのインスタンス
        */
        static Starter* getInstance();

        private:

        static Starter* instance;   // インスタンス


        // タッチセンサ
        device::TouchSensor* touch;

        // ディスプレイ
        device::Display* display;

        //コンストラクタ
        Starter();

    };
}
#endif
