/*
 * @brief 直進走行クラス
 */

#include "StraightRunning.h"

using namespace device;
namespace drive{

    //コンストラクタ
    StraightRunning::StraightRunning(){
        motor_	= device::Motors::getInstance();
    }

    void StraightRunning::run(int speed){
        motor_->setWheelPWM(speed,speed);
    }

    void StraightRunning::initialize(){
        isAccelerating_ = true;
        changeSpeedInitialized_ = false;
    }

    void StraightRunning::run(int targetPwm, int acceleration){
        if (isAccelerating_){
            if (changeSpeed(targetPwm, acceleration)){
                isAccelerating_ = false;
            }
        }
        else{
            int wheelPwm = (motor_->getPWM(MOTOR_LEFT) + motor_->getPWM(MOTOR_RIGHT)) / 2;
            if (targetPwm == wheelPwm){
                run(targetPwm);
            }
            else {
                isAccelerating_ = true;
            }
        }
    }

    bool StraightRunning::changeSpeed(int targetPwm, int acceleration){
        static ev3api::Clock clock = ev3api::Clock();
        static long currentTimeMs = 0;
        static double currentPwm = 0;
        // 値の初期化
        if ( !changeSpeedInitialized_ ){
            changeSpeedInitialized_ = true;
            currentTimeMs = clock.now();
            currentPwm = (motor_->getPWM(MOTOR_LEFT) + motor_->getPWM(MOTOR_RIGHT)) / 2;
            return false;
        }

        // 加速度を絶対値にする
        if (acceleration < 0 ){
            acceleration *= -1;
        }

        // 減速するときは加速度を負の値に
        if (currentPwm > targetPwm ){
            acceleration *= -1;
        }

        // 加速度[pwm/s] * 時間[s]
        double diff = (double)acceleration * (double)(clock.now() - currentTimeMs) / (double)1000;
        currentTimeMs = clock.now();
        currentPwm += diff;

        bool speedChanged = false;

        // 加速のとき
        if (diff > 0.0){
            if (currentPwm >= targetPwm ){
                currentPwm = targetPwm;
                speedChanged = true;
                changeSpeedInitialized_ = false; // 初期化
            }
        }// 減速のとき
        else{
            if (currentPwm <= targetPwm ){
                currentPwm = targetPwm;
                speedChanged = true;
                changeSpeedInitialized_ = false; // 初期化
            }
        }

        run((int)currentPwm);
        return speedChanged;
    }
}
