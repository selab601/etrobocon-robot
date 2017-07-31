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

    bool PolarRunning::runTo(int distance, int polarDegree10, int turnDegree10){
        static int turnDeg10 = 0;
        switch (state_){
            case State::INIT:
                if (isInitialize_){
                    selfPositioin_->startMeasure();
                }
                degreeController_.reset();
                distanceController_.reset();
                targetMm_ = distance;
                targetDegree10_ = polarDegree10;
                turnDeg10 = turnDegree10;
                if (isBack_){   // バックするときは後ろ側を進行方向に向ける
                    turnDeg10 = turnDegree10 <= 0? turnDegree10 +1800: turnDegree10-1800;
                }
                state_ = State::TURN;
                ev3_speaker_play_tone(200, 100);
                break;

            case State::TURN:
                if ( bodyTurn(turnDeg10, 30) ){
                    state_ = State::TRACE;
                    ev3_speaker_play_tone(550, 100);
                }
                break;

            case State::TRACE:
                calculateMaxPwm();
                traceDegree();
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

    bool PolarRunning::runTo(int distance, int degree10){
        return runTo(distance, degree10, degree10);
    }

    bool PolarRunning::runToXY(int xMm, int yMm){
        static int distance = 0;
        static int degree10 = 0;
        switch (xyState_){
            case State::INIT:
                distance = sqrt( xMm*xMm + yMm*yMm );
                degree10 = atan2(yMm, xMm) * 1800 / M_PI;
                xyState_ = State::RUNTO;
                ev3_speaker_play_tone(200, 100);
                break;

            case State::RUNTO:
                if ( runTo(distance, degree10) ){
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

    void PolarRunning::initialize(bool isInitialize){
        isInitialize_ = isInitialize;
    }

    bool PolarRunning::bodyTurn(int degree10, int speed){
        if (isTurnInit_){
            if (isInitialize_){
                bodyAngle_.setBaseAngle();
            }
            degreeController_.setPd();
            isTurnInit_ = false;
        }

        currentDegree10_ = bodyAngle_.getRelative10();
        if (turn(degree10, speed)){
            isTurnInit_ = true;
            return true;
        }
        return false;
    }

    bool PolarRunning::polarTurn(int degree10, int speed){
        static bool isCenterPivot = isCenterPivot_;
        if (isTurnInit_){
            if (isInitialize_){
                selfPositioin_->startMeasure();
            }
            degreeController_.setPd();
            // 極座標を見るときは信地旋回しかしない
            isCenterPivot = isCenterPivot_; // 設定を保存しておく
            isCenterPivot_ = false;
            isTurnInit_ = false;
        }

        currentDegree10_ = selfPositioin_->getPolarTheta10();
        if (turn(degree10, speed)){
            isTurnInit_ = true;
            isCenterPivot_ = isCenterPivot; // フラグを戻す
            return true;
        }
        return false;
    }

    bool PolarRunning::turn(int degree10, int speed){
        int diff = degree10 - currentDegree10_;
        // 差分が0付近のところでいきなり変わらないようにする(359->0とか)
        diff += 1800;
        diff %= 3600;
        diff -= 1800;
        // 足切り
        diff =  150 < diff ?  150 : diff;
        diff = -150 > diff ? -150 : diff;
        int resultspeed = 200 * degreeController_.calculatePid( abs(diff) );
        resultspeed = resultspeed * speed / 90;
        resultspeed += 10;
        resultspeed = speed < resultspeed ? speed : resultspeed;

        int lPwm = 0;
        int rPwm = 0;
        if (0 <= diff){  // 左に回る
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

        if ( abs(diff) < 5 ){
            motors_->setWheelPWM(0, 0);
            return true;
        }
        return false;
    }

    void PolarRunning::traceDegree(){
        int diff = getLeftDeg10();
        // 遠いところほど角度のズレに対する実際の位置のズレが激しくなる
        diff = diff * selfPositioin_->getPolarR();
        diff /= 2;

        degreeController_.setPd(0.000018, 0.01366875);
        int traceMaxPwm = isBack_ ? -traceMaxPwm_ : traceMaxPwm_;
        degreeController_.setMaxPwm(traceMaxPwm);
        degreeController_.run(diff);
    }

    void PolarRunning::calculateMaxPwm(){
        int diff = getLeftMm();
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

    int PolarRunning::getLeftMm(){
        return targetMm_ - selfPositioin_->getPolarR();
    }
    int PolarRunning::getLeftDeg10(){
        int diff = targetDegree10_ - selfPositioin_->getPolarTheta10();
        // 差分が0付近のところでいきなり変わらないようにする(359->0とか)
        while (diff < 0){   // 正の数にしてmod演算する
            diff += 3600;
        }
        diff += 1800;
        diff %= 3600;
        diff -= 1800;
        return diff;
    }
    void PolarRunning::back(bool isBack){
        isBack_ = isBack;
    }
}
