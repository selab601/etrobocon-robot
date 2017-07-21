#include "PolarRunning.h"

namespace drive{
    //コンストラクタ
    PolarRunning::PolarRunning(){
        bodyAngle_ = measurement::BodyAngleMeasurement();
        selfPositioin_ = measurement::SelfPositionEstimation::getInstance();
        motors_ = device::Motors::getInstance();

        state_ = State::INIT;
        xyState_ = State::INIT;
        degreeController_ = PidController();
        distanceController_ = PidController();
    }

    bool PolarRunning::runTo(int distance, int polarDegree, int turnDegree){
        switch (state_){
            case State::INIT:
                selfPositioin_->startMeasure();
                bodyAngle_.setBaseAngle();
                degreeController_.reset();
                distanceController_.reset();
                state_ = State::TURN;
                ev3_speaker_play_tone(200, 100);
                break;

            case State::TURN:
                if ( turn(turnDegree, 40) ){
                    state_ = State::TRACE;
                    ev3_speaker_play_tone(550, 100);
                }
                break;

            case State::TRACE:
                calculateMaxPwm(distance);
                traceDegree(polarDegree);
                if (selfPositioin_->getPolarR() >= distance){
                    state_ = State::FINISHED;
                    ev3_speaker_play_tone(800, 100);
                }
                break;

            case State::FINISHED:
                state_ = State::INIT;
                motors_->setWheelPWM(0, 0);
                return true;

            default:
                return false;
        }
        return false;
    }

    bool PolarRunning::runTo(int distance, int degree){
        return runTo(distance, degree, degree);
    }

    bool PolarRunning::runToXY(int xMm, int yMm){
        static int distance = 0;
        static int degree = 0;
        switch (xyState_){
            case State::INIT:
                distance = sqrt( xMm*xMm + yMm*yMm );
                degree = atan2(yMm, xMm) * 180 / M_PI;
                xyState_ = State::RUNTO;
                ev3_speaker_play_tone(200, 100);
                break;

            case State::RUNTO:
                if ( runTo(distance, degree) ){
                    xyState_ = State::INIT;
                    ev3_speaker_play_tone(1000, 100);
                    return true;
                }
                break;

            default:
                return false;
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
        xyState_ = State::INIT;
        isTurnInit_ = true;
    }

    bool PolarRunning::turn(int degree, int speed){
        if (isTurnInit_){
            bodyAngle_.setBaseAngle();
            degreeController_.setPd();
            isTurnInit_ = false;
        }

        int diff = degree * 10 - bodyAngle_.getResult() * 10;
        diff =  150 < diff ?  150 : diff;
        diff = -150 > diff ? -150 : diff;

        int resultspeed = 200 * degreeController_.calculatePid( abs(diff) );
        resultspeed = resultspeed * speed / 90;
        resultspeed += 10;
        resultspeed = speed < resultspeed ? speed : resultspeed;

        int lPwm = 0;
        int rPwm = 0;
        if (0 <= degree){  // 左に回る
            rPwm = resultspeed;
            // 軸が真ん中でないとき、内側を0にする
            lPwm = isCenterPivot_ ? -resultspeed : 0;
        }
        else{   // 右に回る
            lPwm = resultspeed;
            // 軸が真ん中でないとき、内側を0にする
            rPwm = isCenterPivot_ ? -resultspeed : 0;
        }
        motors_->setWheelPWM(lPwm, rPwm);

        if ( abs(bodyAngle_.getResult()) >= abs(degree) ){
            motors_->setWheelPWM(0, 0);
            isTurnInit_ = true;
            return true;
        }
        return false;
    }

    void PolarRunning::traceDegree(int degree){
        int diff = degree*10 - selfPositioin_->getPolarTheta10();
        // 差分が0付近のところでいきなり変わらないようにする(359->0とか)
        diff += 1800;
        diff %= 3600;
        diff -= 1800;
        // 遠いところほど角度のズレに対する実際の位置のズレが激しくなる
        diff = diff * selfPositioin_->getPolarR();
        diff /= 2;

        // かなり精度良くなった
        degreeController_.setPd(0.0000006, 0.000977);
        degreeController_.setMaxPwm(traceMaxPwm_);
        degreeController_.run(diff);
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
        traceMaxPwm_ += 10;
        // maxPwm_で足切り
        traceMaxPwm_ = maxPwm_ < traceMaxPwm_ ? maxPwm_ : traceMaxPwm_;
    }
}
