/**
 * @file LineTrace.cpp
 * @brief 右・左エッジでのライントレースとエッジの切り替えを行う(予定)
 * @author kuno
 */

#include "./LineTrace.h"
#include "../device/Display.h"

namespace drive{

    LineTrace* LineTrace::instance_ = 0;
    LineTrace::LineTrace()
    {
        motors_ = device::Motors::getInstance();
        colorSensor_ = device::ColorSensor::getInstance();
        clock_ = Clock();
        reset();
        setPid();
    }

    LineTrace* LineTrace::getInstance(){
        if(instance_ == NULL)
        instance_ = new LineTrace();
        return instance_;
    }

    void LineTrace::run(int maxPwm, double relativelyTarget){
        setTarget(relativelyTarget);
        calculatePwm(maxPwm, (int)(calculatePid(colorSensor_->getBrightness(), clock_.now()) * (double)1000) );
    }

    void LineTrace::setPid(double kp, double ki, double kd){
        kp_ = kp;
        ki_ = ki;
        kd_ = kd;
    }

    double LineTrace::getRateByDeltaRad(int deltaRad){
        return 1000.0F / (double)(LINETRACE_TREAD * deltaRad + 1000);
    }

    void LineTrace::calculatePwm(int maxPwm, int deltaRad){

        int lPwm;
        int rPwm;

        if (deltaRad < 0 ){
            deltaRad *= -1;
            lPwm = maxPwm;
            rPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
        }
        else{
            rPwm = maxPwm;
            lPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
        }

        motors_->setPWM(device::MOTOR_LEFT, lPwm);
        motors_->setPWM(device::MOTOR_RIGHT, rPwm);

    }

    double LineTrace::calculatePid(int brightness, int timeMs){
        counter_++;
        diff_[1] = diff_[0];
        timeMs_[1] = timeMs_[0];

        diff_[0] = brightness*10 - target_;
        timeMs_[0] = timeMs;

        int timeDiff = timeMs_[1] - timeMs_[0];

        // 積分の計算
        integrated_ += timeDiff * (diff_[1] + diff_[0]) / 2;

        // I、D制御の情報が揃っていない時、P制御の値を返す
        double turn;
        if (counter_ < 2){
            turn = kp_ * (double)diff_[0];
        }
        else{
            turn = kp_ * (double)diff_[0] +
                   ki_ * (double)integrated_ +
                   kd_ * (double)(diff_[1] - diff_[0]) / (double)timeDiff;
        }
        return turn;
    }

    void LineTrace::setTarget(double relativelyTarget){

        blackValue_ = 10 * colorSensor_->getBlackCalibratedValue();
        whiteValue_ = 10 * colorSensor_->getWhiteCalibratedValue();

        if(relativelyTarget <= 0.0 || 1.0 <= relativelyTarget){
            target_ = blackValue_ + (whiteValue_ - blackValue_) * DEFAULT_TARGET;
        }
        else{
            target_ = blackValue_ + (whiteValue_ - blackValue_) * relativelyTarget;
        }
    }

    void LineTrace::reset(){
        counter_ = 0;
        integrated_ = 0;
        diff_[1] = diff_[0] = 0;
        timeMs_[1] = timeMs_[0] = 0;
    }

};
