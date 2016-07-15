/**
 * @file LineTrace.h
 * @brief ライントレースクラス
 * @details setPid(),setTarget()で各種設定を行った後にrun()で走行
 * @author kuno
 */

#ifndef LINE_TRACE_
#define LINE_TRACE_

#include <math.h>
#include "../device/ColorSensor.h"
#include "../device/Motors.h"
#include <Clock.h>

#define DEFAULT_KP          0.0144F /* PID処理のデフォルトのP値 */
#define DEFAULT_KI          0.0F    /* PID処理のデフォルトのI値 */
#define DEFAULT_KD          0.72F   /* PID処理のデフォルトのD値 */
#define DEFAULT_TARGET      0.6F    /* 明るさセンサの目標値となる値の黒の割合のデフォルト値*/

#define LINETRACE_TREAD      1      /*未使用 きちんとした角速度に計算する定数*/

using namespace ev3api;

namespace drive{
    class LineTrace{
    private:
        static LineTrace* instance_;
        LineTrace();

        // キャリブレーション値
        int whiteValue_;            //白のキャリブレーション値を10倍したもの
        int blackValue_;            //黒のキャリブレーション値を10倍したもの

        int target_ = 0;            // ターゲット値：ターゲット目標値を元に算出される(明るさセンサの値を10倍した時の)光センサの目標値

        int diff_[2];               // 明るさの値を10倍し、ターゲット値との差分をとったもの
        int timeMs_[2];
        int integrated_ = 0;
        int counter_ = 0;

        int maxPwm_;

        double  kp_;
        double  ki_;
        double  kd_;

        // Device
        device::ColorSensor* colorSensor_;
        device::Motors* motors_;
        Clock clock_;


    public:

        /**
         * @brief インスタンスを取得する
         *
         * @return インスタンスのポインタ
         * @author Nagaoka
         */
        static LineTrace* getInstance();

        /**
         * @brief ライントレースを行う
         *
         * @param maxPwm モータのPWMの最大値
         * @param target ターゲット目標値 ( Black 0.0 < target < 1.0 White) default:0.6
         * @author Nagaoka
         */
        void run(int maxPwm,double relativeTarget = DEFAULT_TARGET);

        /**
         * @brief PIDパラーメータをセットする
         * @param kp P制御の係数
         * @param ki I制御の係数
         * @param kd D制御の係数
         * @sa run
         * @author Nagaoka
         */
        void setPid(double kp = DEFAULT_KP, double ki = DEFAULT_KI, double kd = DEFAULT_KD);

        /**
         * @brief ターゲット目標値をセットする(≠ターゲット値)
         * @details (白)0.0 < x < 1.0(黒) の値から、ターゲット目標値をセットする
                    x ≦ 0.0 || 1.0 ≦ x の場合 default値(0.6)を設定
         * @author Nagaoka
         **/
        void setTarget(double relativeTarget = DEFAULT_TARGET);

        /**
         * @brief PID制御の内部の情報をリセットする
         * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する
         * @author Nagaoka
         **/
        void reset();

    private:

        /**
         * @brief PWMの最大値、車体の角速度からモータのPWMをセットする
         *
         * @param maxPwm モータのPWMの最大値
         * @param deltaRad 角速度[rad / 内側のタイヤが進んだ距離] 左側に曲がるときが正の値
         * @author Nagaoka
         */
        void calculatePwm(int maxPwm, int deltaRad);

        /**
         * @brief PID制御の計算を行う
         * @details ターゲット値よりも黒寄りにいる時、負の値を返す
         * @author Nagaoka
         **/
        double calculatePid(int brightness, int timeMs);

        /**
         * @brief 内側のタイヤが進んだ距離あたりの角度の変化[milli rad]から、外側のタイヤの速さに対する内側のタイヤの速さの比率を返す
                  ソース内の計算式は上記の省略(証明略)
         * @details
         * @param deltaRad 内側のタイヤが進んだ距離あたりの車体の角度の変化(deltarad >= 0)[milli rad]
         * @author Nagaoka
         **/
        double getRateByDeltaRad(int deltaRad);

    }; //end of class
};
#endif
