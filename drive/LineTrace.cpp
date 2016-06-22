/**
 * @file LineTrace.cpp
 * @brief ライントレースクラス
 * @date 2016/6/22
 * @author Nagaoka
 */

#include "./LineTrace.h"

namespace drive{

    /**
     * @brief PID制御の計算を行う
     * @details ターゲット値よりも黒寄りにいる時、
     * @author Nagaoka
     **/
    float_t LineTrace::calculatePid(int brightness, int timeMs){
        counter++;
        mDiff[1] = mDiff[0];
        mTimeMs[1] = mTimeMs[0];
        mDiff[0] = brightness*10 - mTarget;
        mTimeMs[0] = timeMs;

        int timeDiff = mTimeMs[1] - mTimeMs[0];

        // 積分の計算
        mIntegrated += timeDiff * (mDiff[1] + mDiff[0]) / 2;

        // I、D制御の情報が揃っていない時、P制御の値を返す
        if (counter < 2)
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
        mTarget = mBlackValue + (mWhiteValue - mBlackValue) * target;
    }

    /**
     * @brief PID制御の内部の情報をリセットする
     * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する
     * @author Nagaoka
     **/
    void LineTrace::reset(){
        counter = 0;
        mIntegrated = 0;
        mDiff[1] = mDiff[0] = 0;
        mTimeMs[1] = mTimeMs[0] = 0;
    }

};

