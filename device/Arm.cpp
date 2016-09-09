#include "Arm.h"

namespace device
{
    // インスタンス変数初期化
    Arm* Arm::instance_ = 0;

    // シングルトン
    Arm* Arm::getInstance(){
        if(instance_ == 0){
            instance_ = new Arm();
        }
        return instance_;
    }

    Arm::Arm() :Motor(PORT_B) // ポートの指定
    {
        reset();
    }

    bool Arm::setDegree(int degree, int maxPwm){
        int diffDegree = degree - getCount();
        int absDiffDigree = 0 < diffDegree? diffDegree: -diffDegree;
        int pwm = absDiffDigree + 5;
        pwm = pwm > maxPwm? maxPwm: pwm;    // maxPwmで足切り

        if (0 > diffDegree){
            pwm *= -1;
        }
        setPWM(pwm);

        if (absDiffDigree <= 1){
            stop();
            return true;
        }
        return false;
    }

    bool Arm::up(int maxPwm){
        return setDegree(70, maxPwm);
    }

    bool Arm::down(int maxPwm){
        return setDegree(-20, maxPwm);
    }

    bool Arm::normal(int maxPwm){
        return setDegree(0, maxPwm);
    }

}
