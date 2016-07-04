/**
 * @file LineTrace.cpp
 * @brief ライントレースクラス
 * @date 2016/6/22
 * @author Nagaoka
 */

#include "./LineTrace.h"

#include "../device/Display.h"

namespace drive{

    LineTrace* LineTrace::Instance = 0;
    LineTrace::LineTrace()
    {
        Motors = device::Motors::getInstance();
        Color = device::ColorSensor::getInstance();
        Clock = Clock();
        reset();
        setPID();
    }

    LineTrace* LineTrace::getInstance(){
        if (NULL == Instance)
            Instance = new LineTrace();
        return Instance;
    }


    void LineTrace::run(int maxPwm, double target){
        if ( target > 0)
            setTarget(target);
        else
        if (Target == 0)
            setTarget(DEFAULT_TARGET);

        setPwm(maxPwm, (int)
                (calculatePid(Color->getBrightness(), Clock.now()) * (double)1000) );
    }

    void LineTrace::setPID(double kp, double ki, double kd){
        this.Kp = kp;
        this.Ki = ki;
        this.Kd = kd;
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

        Motors->setPWM(device::MOTOR_LEFT, lPwm);
        Motors->setPWM(device::MOTOR_RIGHT, rPwm);

        shippoPwm /= 3;
        if (shippoPwm > 100)
            shippoPwm = 100;
        if (shippoPwm < -100)
            shippoPwm = -100;

        Motors->setPWM(device::MOTOR_TAIL, shippoPwm);
    }

    /**
     * @brief PID制御の計算を行う
     * @details ターゲット値よりも黒寄りにいる時、
     * @author Nagaoka
     **/
    double LineTrace::calculatePid(int brightness, int timeMs){
        Counter++;
        Diff[1] = Diff[0];
        TimeMs[1] = TimeMs[0];
        Diff[0] = brightness*10 - Target;
        TimeMs[0] = timeMs;

        int timeDiff = TimeMs[1] - TimeMs[0];

        // 積分の計算
        Integrated += timeDiff * (Diff[1] + Diff[0]) / 2;

        // Debug
        device::Display::getInstance()->updateDisplay("brightness:", brightness, 8);
        device::Display::getInstance()->updateDisplay("Target:", Target, 9);


        double turn;
        // I、D制御の情報が揃っていない時、P制御の値を返す
        if (Counter < 2)
            turn =  Kp * (double)Diff[0];
        else
            turn =  Kp * (double)Diff[0] +
                Ki * (double)Integrated +
                Kd * (double)(Diff[1] - Diff[0]) / (double)timeDiff;

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
        BlackValue = 10 * Color->getBlackCalibratedValue();
        WhiteValue = 10 * Color->getWhiteCalibratedValue();
        Target = BlackValue + (WhiteValue - BlackValue) * target;
    }



    /**
     * @brief PID制御の内部の情報をリセットする
     * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する
     * @author Nagaoka
     **/
    void LineTrace::reset(){
        Counter = 0;
        Integrated = 0;
        Diff[1] = Diff[0] = 0;
        TimeMs[1] = TimeMs[0] = 0;
    }

};

