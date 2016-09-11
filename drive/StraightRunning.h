/*
 * @brief 直進走行クラス
 */

#ifndef STRAIGHT_RUNNING_H_
#define STRAIGHT_RUNNING_H_

#include "device/Motors.h"
#include "ev3api.h"
#include <Clock.h>

namespace drive{

    class StraightRunning{

        private:
            device::Motors* motor_;
            bool isAccelerating_ = true;
            bool changeSpeedInitialized_ = false;

        public:
            //コンストラクタ
            StraightRunning();

            /**
             * @brief 直進する．
             * @param speed (-100~100)．	正の値で前進，負の値で後退
             */
            void run(int speed);

            /**
             * @brief 加速の情報を初期化する
             * @detail 全開に加速の途中でrun,changeSpeedをやめたときには初期化しなければならない
             */
            void initialize();


            /**
             * @brief 加速度を設定して直進する
             *
             * @param targetPwm 目標のPWM値(-100 ~ 100)
             * @param acceleration 加速度の絶対値[pwm/s]
             */
            void run(int targetPwm, int acceleration);

            /**
             * @brief 加速度を設定して速度を変える
             *
             * @param targetPwm 目標のPWM値(-100 ~ 100)
             * @param acceleration 加速度の絶対値[pwm/s]
             *
             * @return 目標のPWM値に達したらtrue
             */
            bool changeSpeed(int targetPwm, int acceleration);
    };
};
#endif
