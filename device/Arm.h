#ifndef HACKEV_ARM_H
#define HACKEV_ARM_H

#include "ev3api.h"
#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/Motor.h"
#include "../measurement/TimeMeasurement.h"

#define ARM_ANGLE 42        // アームの初期角度（アームが下側にぶつかっている状態からの角度(degree)
#define ARM_MAX_PWM 30

namespace device
{
    class Arm:public ev3api::Motor
    {
    private:
        //! 共有インスタンス
        static Arm* instance_;

        // アームを真下に向ける時の状態
        enum class ArmSettingState{
            INIT,
            PULL,
            PUSH,
            FINISHED,
        };

    private:
        Arm();

    public:
        static Arm* getInstance();


        /**
         * @brief アームを指定した角度に向ける
         *
         * @param degree アームの角度[degree]
         * @param maxPwm 与えるPWMの最大値
         *
         * @return 終了したらtrue
         */
        bool setDegree(int degree, int maxPwm = ARM_MAX_PWM);

        /**
         * @brief アームを上に向ける
         *
         * @param maxPwm 与えるPWMの最大値
         *
         * @return 終了したらtrue
         */
        bool up(int maxPwm = ARM_MAX_PWM);

        /**
         * @brief アームを下に向ける（ライントレースできない）
         *
         * @param maxPwm 与えるPWMの最大値
         *
         * @return 終了したらtrue
         */
        bool down(int maxPwm = ARM_MAX_PWM);

        /**
         * @brief アームを真下に向ける（ライントレース用)
         *
         * @param maxPwm 与えるPWMの最大値
         *
         * @return 終了したらtrue
         */
        bool normal(int maxPwm = ARM_MAX_PWM);

        /**
         * @brief アームを真下に向ける
         *
         * @return 終了した時true
         */
        bool reset();

    };
}

#endif
