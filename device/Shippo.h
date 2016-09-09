#ifndef SHIPPO_H
#define SHIPPO_H

#include "ev3api.h"
#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/Motor.h"

#define SHIPPO_MAX_PWM 30

namespace device
{
    class Shippo:public ev3api::Motor
    {
    private:
        //! 共有インスタンス
        static Shippo* instance_;

    private:
        Shippo();

    public:
        static Shippo* getInstance();


        /**
         * @brief しっぽふりふり
         * @detail 可愛くしっぽふりふり
         */
        void furifuri(int maxPwm = SHIPPO_MAX_PWM);

        /**
         * @brief しっぽを指定した角度に向ける
         *
         * @param degree しっぽの角度[degree]
         *
         * @return 終了したらtrue
         */
        bool setDegree(int degree, int maxPwm = SHIPPO_MAX_PWM);

        /**
         * @brief 喜んでる(しっぽを上に向ける)
         *
         * @return 終了したらtrue
         */
        bool preased(int maxPwm = SHIPPO_MAX_PWM);

        /**
         * @brief 退屈してる(しっぽを下に向ける)
         *
         * @return 終了したらtrue
         */
        bool bored(int maxPwm = SHIPPO_MAX_PWM);

    };
}

#endif
