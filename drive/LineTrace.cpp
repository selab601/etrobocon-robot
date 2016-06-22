/**
 * @file LineTrace.cpp
 * @brief ライントレースクラス
 * @date 2016/6/22
 * @author Nagaoka
 */

#include "./LineTrace.h"

namespace drive{

    LineTrace* LineTrace::mInstance = 0;
    LineTrace::LineTrace():
        mLMotor(PORT_A),
        mRMotor(PORT_B){
        mColor = Color::getInstance();
        mLMotor = Motor(PORT_A);
        mRMotor = Motor(PORT_B);
        mClock = Clock();
        reset();
    }

    LineTrace* LineTrace::getInstance(){
        if (NULL == mInstance)
            mInstance = new LineTrace();
        return mInstance;
    }


    void LineTrace::run(int maxPwm, float_t target){
        if ( target > 0)
            setTarget(target);
        else
        if (mTarget == 0)
            setTarget(DEFAULT_TARGET);

        setPwm(maxPwm, (int)
                (calculatePid(mColor->getBrightness(), mClock.now()) * (float)100) );
    }

    void LineTrace::setPID(float_t kp, float_t ki, float_t kd){
        mKp = kp;
        mKi = ki;
        mKd = kd;
    }

    float_t LineTrace::getRateByDeltaRad(int deltaRad){
        return 100 / (TREAD * deltaRad + 100);
    }

    void LineTrace::setPwm(int maxPwm, int deltaRad){
        int lPwm;
        int rPwm;
        if (deltaRad < 0 ){
            deltaRad *= -1;
            lPwm = maxPwm;
            rPwm = getRateByDeltaRad(deltaRad) * maxPwm;
        }
        else{
            rPwm = maxPwm;
            lPwm = getRateByDeltaRad(deltaRad) * maxPwm;
        }
        mLMotor.setPWM(lPwm);
        mRMotor.setPWM(rPwm);
    }

    /**
     * @brief PID制御の計算を行う
     * @details ターゲット値よりも黒寄りにいる時、
     * @author Nagaoka
     **/
    float_t LineTrace::calculatePid(int brightness, int timeMs){
        mCounter++;
        mDiff[1] = mDiff[0];
        mTimeMs[1] = mTimeMs[0];
        mDiff[0] = brightness*10 - mTarget;
        mTimeMs[0] = timeMs;

        int timeDiff = mTimeMs[1] - mTimeMs[0];

        // 積分の計算
        mIntegrated += timeDiff * (mDiff[1] + mDiff[0]) / 2;

        // I、D制御の情報が揃っていない時、P制御の値を返す
        if (mCounter < 2)
            return  mKp * (float_t)mDiff[0];
        else
            return  mKp * (float_t)mDiff[0] +
                mKi * (float_t)mIntegrated +
                mKd * (float_t)(mDiff[1] - mDiff[0]) / (float_t)timeDiff;
    }

    /**
     * @brief ターゲット値をセットする
     * @details 0.0 ~ 1.0 の値から、ターゲット値をセットする
     * @author Nagaoka
     **/
    void LineTrace::setTarget(float_t target){
        mBlackValue = mColor->getBlackCalibratedValue();
        mWhiteValue = mColor->getWhiteCalibratedValue();
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

