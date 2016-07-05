/**
 * @file LineTrace.cpp
 * @brief ライントレースクラス
 * @date 2016/6/22
 * @author Nagaoka
 */

#include "./LineTrace.h"

#include "../device/Display.h"

namespace drive{

    LineTrace* LineTrace::mInstance = 0;
    LineTrace::LineTrace()
    {
        mMotors = device::Motors::getInstance();
        mColor = device::ColorSensor::getInstance();
        mClock = Clock();
        reset();
        setPID();
    }

    LineTrace* LineTrace::getInstance(){
        if (NULL == mInstance)
            mInstance = new LineTrace();
        return mInstance;
    }


    void LineTrace::run(int maxPwm, double target){
        if ( target > 0)
            setTarget(target);
        else
        if (mTarget == 0)
            setTarget(DEFAULT_TARGET);

        setPwm(maxPwm, (int)
                (calculatePid(mColor->getBrightness(), mClock.now()) * (double)1000) );
    }

    void LineTrace::setPID(double kp, double ki, double kd){
        mKp = kp;
        mKi = ki;
        mKd = kd;
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

        mMotors->setPWM(device::MOTOR_LEFT, lPwm);
        mMotors->setPWM(device::MOTOR_RIGHT, rPwm);

        shippoPwm /= 3;
        if (shippoPwm > 100)
            shippoPwm = 100;
        if (shippoPwm < -100)
            shippoPwm = -100;

        mMotors->setPWM(device::MOTOR_TAIL, shippoPwm);
    }

    /**
     * @brief PID制御の計算を行う
     * @details ターゲット値よりも黒寄りにいる時、
     * @author Nagaoka
     **/
    double LineTrace::calculatePid(int brightness, int timeMs){
        mCounter++;
        mDiff[1] = mDiff[0];
        mTimeMs[1] = mTimeMs[0];
        mDiff[0] = brightness*10 - mTarget;
        mTimeMs[0] = timeMs;

        int timeDiff = mTimeMs[1] - mTimeMs[0];

        // 積分の計算
        mIntegrated += timeDiff * (mDiff[1] + mDiff[0]) / 2;

        // Debug
        device::Display::getInstance()->updateDisplay("brightness:", brightness, 8);
        device::Display::getInstance()->updateDisplay("mTarget:", mTarget, 9);


        double turn;
        // I、D制御の情報が揃っていない時、P制御の値を返す
        if (mCounter < 2)
            turn =  mKp * (double)mDiff[0];
        else
            turn =  mKp * (double)mDiff[0] +
                mKi * (double)mIntegrated +
                mKd * (double)(mDiff[1] - mDiff[0]) / (double)timeDiff;

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
        mBlackValue = 10 * mColor->getBlackCalibratedValue();
        mWhiteValue = 10 * mColor->getWhiteCalibratedValue();
        mTarget = mBlackValue + (mWhiteValue - mBlackValue) * target;
    }



    /**
     * @brief PID制御の内部の情報をリセットする
     * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する
     * @author Nagaoka
     **/
    void LineTrace::reset(){
        mCounter = 0;
        mIntegrated = 0;
        mDiff[1] = mDiff[0] = 0;
        mTimeMs[1] = mTimeMs[0] = 0;
    }

};

