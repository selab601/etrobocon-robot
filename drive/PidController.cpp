#include "PidController.h"

namespace drive{

    PidController::PidController()
    {
        motors_ = device::Motors::getInstance();
        clock_ = ev3api::Clock();
        reset();
        setMaxPwm();
        setPid();
    }

    void PidController::setMaxPwm(int maxPwm){
        maxPwm_ = maxPwm;
    }
    void PidController::setPid(double kp, double ki, double kd){
        kp_ = kp;
        ki_ = ki;
        kd_ = kd;
    }

    void PidController::setPd(double kp, double kd){
        setPid(kp, 0, kd);
    }
    void PidController::run(int controllTarget){
        calculatePwm( 1000 * calculatePid(controllTarget) );

        motors_->setPWM(device::MOTOR_LEFT, lPwm_);
        motors_->setPWM(device::MOTOR_RIGHT, rPwm_);
    }

    double PidController::getRateByDeltaRad(int deltaRad){
        return 1000.0 / (double)(PID_TREAD * deltaRad + 1000);
    }

    void PidController::calculatePwm(int deltaRad){
        if (deltaRad < 0 ){
            deltaRad *= -1;
            lPwm_ = maxPwm_;
            rPwm_ = getRateByDeltaRad(deltaRad) * (double)maxPwm_;
        }
        else{
            rPwm_ = maxPwm_;
            lPwm_ = getRateByDeltaRad(deltaRad) * (double)maxPwm_;
        }
    }

    double PidController::calculatePid(int controllTarget){
        // 1つ前の値:[1], 現在の値:[0]
        targetValue_[1] = targetValue_[0];
        timeMs_[1] = timeMs_[0];

        targetValue_[0] = controllTarget;
        timeMs_[0] = clock_.now();

        int timeDiff = timeMs_[0] - timeMs_[1];


        // I、D制御の情報が揃っていない時、P制御の値を返す
        double turn;
        if (usePid_ == false){
            turn = kp_ * (double)targetValue_[0];
            usePid_ = true;
        }
        else{
            // 積分の計算
            // timeDiff が正確な値になってから積分値を計算する
            integrated_ += timeDiff * (targetValue_[1] + targetValue_[0]) / 2;

            turn = kp_ * (double)targetValue_[0] +
                   ki_ * (double)integrated_ +
                   kd_ * (double)(targetValue_[0] - targetValue_[1]) / (double)timeDiff;
        }
        return turn;
    }

    void PidController::reset(){
        usePid_  = false;
        integrated_ = 0;
        targetValue_[1] = targetValue_[0] = 0;
        timeMs_[1] = timeMs_[0] = clock_.now();
    }

};
