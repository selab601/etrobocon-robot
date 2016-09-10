#include "Arm.h"

using namespace measurement;

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
        return setDegree(80, maxPwm);
    }

    bool Arm::down(int maxPwm){
        return setDegree(-15, maxPwm);
    }

    bool Arm::normal(int maxPwm){
        return setDegree(0, maxPwm);
    }


    bool Arm::reset(){
        static TimeMeasurement timeMeasurement = TimeMeasurement();
        static ArmSettingState state = ArmSettingState::INIT;
        static int baseCount = 0;
        switch(state){
            case ArmSettingState::INIT:
                timeMeasurement.setTargetTime(800);
                timeMeasurement.setBaseTime();
                state = ArmSettingState::PULL;
                break;

            case ArmSettingState::PULL:
                setPWM(-40);
                if (timeMeasurement.getResult()){
                    state = ArmSettingState::PUSH;
                    setPWM(0);
                    baseCount = getCount();
                }
                break;

            case ArmSettingState::PUSH:
                {
                    int currentCount = getCount();
                    int pwm = ARM_ANGLE - (currentCount - baseCount);
                    setPWM(pwm);

                    if (ARM_ANGLE == (currentCount - baseCount)){
                        state = ArmSettingState::FINISHED;
                        Motor::reset(); // モータのエンコーダ値をリセット
                        ev3_speaker_play_tone(500,100); // 終わった時に音を出す
                    }
                }
                break;

            case ArmSettingState::FINISHED:
                state = ArmSettingState::INIT;
                return true;
                break;
        }
        return false;
    }

}
