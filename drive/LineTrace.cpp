/**
 * @file LineTrace.cpp
 * @brief ライントレースクラス
 * @date 2016/6/22
 * @author Nagaoka
 */

#include "./LineTrace.h"

#include "../device/Display.h"

namespace drive{

    LineTrace* LineTrace::instance = 0;
    LineTrace::LineTrace()
    {
        motors = device::Motors::getInstance();
        color = device::ColorSensor::getInstance();
        clock = Clock();
        reset();
        setPID();
    }

    LineTrace* LineTrace::getInstance(){
        if (NULL == instance)
            instance = new LineTrace();
        return instance;
    }

    void LineTrace::run(int maxPwm, double target){
        if ( target > 0)
            setTarget(target);
        else
        if (target == 0)
            setTarget(DEFAULT_TARGET);

        setPwm(maxPwm, (int)
                (calculatePid(color->getBrightness(), clock.now()) * (double)1000) );
    }

    void LineTrace::setPID(double kp, double ki, double kd){
        kp = kp;
        ki = ki;
        kd = kd;
    }

    double LineTrace::getRateByDeltaRad(int deltaRad){
        return 1000.0F / (double)(TREAD * deltaRad + 1000);
    }


    void LineTrace::setPwm(int maxPwm, int deltaRad){
        int lPwm;
        int rPwm;
        int shippoPwm = deltaRad;

        if (deltaRad < 0 ){
            deltaRad *= -1;
            lPwm = maxPwm;
            rPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
        }
        else{
            rPwm = maxPwm;
            lPwm = getRateByDeltaRad(deltaRad) * (double)maxPwm;
        }

        motors->setPWM(device::MOTOR_LEFT, lPwm);
        motors->setPWM(device::MOTOR_RIGHT, rPwm);

        shippoPwm /= 3;
        if (shippoPwm > 100)
            shippoPwm = 100;
        if (shippoPwm < -100)
            shippoPwm = -100;

    }

    /**
     * @brief PID制御の計算を行う
     * @details ターゲット値よりも黒寄りにいる時、
     * @author Nagaoka
     **/
    double LineTrace::calculatePid(int brightness, int timeMs){
        counter++;
        diff[1] = diff[0];
        this->timeMs[1] = this->timeMs[0];
        diff[0] = brightness*10 - target;
        this->timeMs[0] = timeMs;

        int timeDiff = this->timeMs[1] - this->timeMs[0];

        // 積分の計算
        integrated += timeDiff * (diff[1] + diff[0]) / 2;

        // Debug
        device::Display::getInstance()->updateDisplay("brightness:", brightness, 8);
        device::Display::getInstance()->updateDisplay("Target:", target, 9);


        double turn;
        // I、D制御の情報が揃っていない時、P制御の値を返す
        if (counter < 2)
            turn =  kp * (double)diff[0];
        else
            turn =  kp * (double)diff[0] +
                ki * (double)integrated +
                kd * (double)(diff[1] - diff[0]) / (double)timeDiff;

        // Debug
        device::Display::getInstance()->updateDisplay("pid turn:", turn * 1000.0F, 10);
        return turn;
    }

    /**
     * @brief ターゲット値をセットする
     * @details 0.0 ~ 1.0 の値から、ターゲット値をセットする
     * @author Nagaoka
     **/
    void LineTrace::setTarget(double target){
        blackValue = 10 * color->getBlackCalibratedValue();
        whiteValue = 10 * color->getWhiteCalibratedValue();
        target = blackValue + (whiteValue - blackValue) * target;
    }

    /**
     * @brief PID制御の内部の情報をリセットする
     * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する
     * @author Nagaoka
     **/
    void LineTrace::reset(){
        counter = 0;
        integrated = 0;
        diff[1] = diff[0] = 0;
        timeMs[1] = timeMs[0] = 0;
    }

};
