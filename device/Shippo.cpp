#include "Shippo.h"

namespace device
{
    // インスタンス変数初期化
    Shippo* Shippo::instance_ = 0;

    // シングルトン
    Shippo* Shippo::getInstance(){
        if(instance_ == 0){
            instance_ = new Shippo();
        }
        return instance_;
    }

    Shippo::Shippo() :Motor(PORT_C) // ポートの指定
    {
        reset();
    }

    void Shippo::furifuri(int maxPwm){
        static bool isLeftFurifuri = true;
        switch(isLeftFurifuri){
            case true:
            if (setDegree(180 + 45, maxPwm)){
                isLeftFurifuri = !isLeftFurifuri;
            }
            break;

            case false:
            if (setDegree(180 - 45, maxPwm)){
                isLeftFurifuri = !isLeftFurifuri;
            }
            break;
        }
    }

    bool Shippo::setDegree(int degree, int maxPwm){
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

    bool Shippo::pleased(int maxPwm){
        return setDegree(-180, maxPwm);
    }

    bool Shippo::bored(int maxPwm){
        return setDegree(0, maxPwm);
    }


}
