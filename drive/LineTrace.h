/**
 * @file LineTrace.h
 * @brief ライントレースクラス
 * @date 2015/07/29
 * @author Nagaoka
 */

#ifndef LINE_TRACE_
#define LINE_TRACE_

#include <math.h>
#include "../device/Color.h"
#include <Motor.h>
#include <Clock.h>

#define DEFAULT_KP          0.0144F /* PID処理のデフォルトのP値 */
#define DEFAULT_KI          0.0F    /* PID処理のデフォルトのI値 */
#define DEFAULT_KD          0.72F   /* PID処理のデフォルトのD値 */
#define DEFAULT_TARGET      0.6F

#define TREAD               1

using namespace ev3api;

namespace drive{
    class LineTrace{
    private:
        static LineTrace* mInstance;

        LineTrace();


        // キャリブレーション値
        int mWhiteValue;
        int mBlackValue;

        int mTarget = 0;        // 明るさセンサの値を10倍したときのターゲット値

        int mDiff[2];       // 明るさの値を10倍し、ターゲット値との差分をとったもの
        int mTimeMs[2];
        int mIntegrated = 0;
        int mCounter = 0;

        int mMaxPwm;

        float_t mKp;
        float_t mKi;
        float_t mKd;

        // Device
        Color* mColor;
        Motor mLMotor;
        Motor mRMotor;
        Clock mClock;


    public:

        static LineTrace* getInstance();

        void run(int maxPwm, float_t target = 0);

        void setPID(float_t kp = DEFAULT_KP, float_t ki = DEFAULT_KI, float_t kd = DEFAULT_KD);

        /**
         * @brief PID制御の計算を行う
         * @details ターゲット値よりも黒寄りにいる時、負の値を返す
         * @author Nagaoka
         **/
        float_t calculatePid(int brightness, int timeMs);


        /**
         * @brief 進んだ距離あたりの角度の変化（Milli rad)外側のタイヤの速さに対する内側のタイヤの速さの比率を返す
         * @details
         * @param deltaRad 進んだ距離あたりの角度の変化(Milli rad)
         * @author Nagaoka
         **/
        float_t getRateByDeltaRad(int deltaRad);

        int getLowerPwmByTimeDifferential(int deltaRad, int higherPwm);

        void setPwm(int maxPwm, int deltaRad);

        /**
         * @brief ターゲット値をセットする
         * @details 0.0 ~ 1.0 の値から、ターゲット値をセットする
         * @author Nagaoka
         **/
        void setTarget(float_t target);

        /**
         * @brief PID制御の内部の情報をリセットする
         * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する
         * @author Nagaoka
         **/
        void reset();

    }; //end of class
};

#endif
