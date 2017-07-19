#include "PolarRunning.h"

namespace drive{
    //コンストラクタ
    PolarRunning::PolarRunning(){
        bodyAngle_ = measurement::BodyAngleMeasurement();
        selfPositioin_ = measurement::SelfPositionEstimation::getInstance();
        motors_ = device::Motors::getInstance();

        state_ = State::INIT;
        degreeController_ = PidController();
        distanceController_ = PidController();
    }

    bool PolarRunning::runTo(int distance, int degree){
        switch (state_){
            case State::INIT:
                selfPositioin_->startMeasure();
                bodyAngle_.setBaseAngle();
                degreeController_.reset();
                distanceController_.reset();
                state_ = State::TURN;
                break;

            case State::TURN:
                if ( turn(degree, 40) ){
                    state_ = State::TRACE;
                }
                break;

            case State::TRACE:
                calculateMaxPwm(distance);
                traceDegree(degree);
                if (selfPositioin_->getPolarR() >= distance){
                    state_ = State::FINISHED;
                }
                break;

            case State::FINISHED:
                state_ = State::INIT;
                motors_->setWheelPWM(0, 0);
                return true;
        }
        return false;
    }

    void PolarRunning::setMaxPwm(int maxPwm){
        maxPwm_ = maxPwm;
    }
    void PolarRunning::centerPivot(bool isCenterPivot){
        isCenterPivot_ = isCenterPivot;
    }
    void PolarRunning::reset(){
        state_ = State::INIT;
    }

    bool PolarRunning::turn(int degree, int speed){
        int lPwm = 0;
        int rPwm = 0;
        if (0 <= degree){  // 左に回る
            rPwm = speed;
            // 軸が真ん中でないとき、内側を0にする
            lPwm = isCenterPivot_ ? -speed : 0;
        }
        else{   // 右に回る
            lPwm = speed;
            // 軸が真ん中でないとき、内側を0にする
            rPwm = isCenterPivot_ ? -speed : 0;

            degree *= -1;   // 角度を絶対値にしておく
        }
        motors_->setWheelPWM(lPwm, rPwm);

        if (bodyAngle_.getResult() >= degree){
            motors_->setWheelPWM(0, 0);
            return true;
        }
        return false;
    }

    void PolarRunning::traceDegree(int degree){
        int diff = degree*10 - selfPositioin_->getPolarTheta10();
        diff *= 2;
        // 40で足切り
        diff =  40 < diff ?  40 : diff;
        diff = -40 > diff ? -40 : diff;

        degreeController_.setPid();
        degreeController_.setMaxPwm(traceMaxPwm_);
        degreeController_.run(diff*10);
    }

    void PolarRunning::calculateMaxPwm(int distance){
        int diff = distance - selfPositioin_->getPolarR();
        if (50 < diff){
            distanceController_.setPid(0.003, 0.000000033, 0.4); // PID制御
        }
        else {
            distanceController_.setPd(0.003, 0.4);// PD制御
        }

        // 50で足切り (最後の5cmだけ制御)
        diff =  50 < diff ?  50 : diff;
        diff = -50 > diff ? -50 : diff;

        traceMaxPwm_ = 600 * distanceController_.calculatePid(diff);
        traceMaxPwm_ = traceMaxPwm_ * maxPwm_ / 90;
        // maxPwm_で足切り
        traceMaxPwm_ = maxPwm_ < traceMaxPwm_ ? maxPwm_ : traceMaxPwm_;
    }
}
