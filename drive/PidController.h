#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include "device/Motors.h"
#include <Clock.h>

//ほぼ直線用のデフォルト値
//linetrace_->setPid();でこの値になる。
#define PID_DEFAULT_KP          0.003F /* PID処理のデフォルトのP値 */
#define PID_DEFAULT_KI          0.00000003333333F    /* PID処理のデフォルトのI値 */
#define PID_DEFAULT_KD          0.2F   /* PID処理のデフォルトのD値 */

#define PID_DEFAULT_MAXPWM      80      /* デフォルトのmaxPwm値*/
#define PID_TREAD      1      /*未使用 きちんとした角速度に計算する定数*/



namespace drive{

    class PidController{
    private:

        // 1つ前の値:[1], 現在の値:[0]
        int targetValue_[2];    // 制御対象となる値
        int timeMs_[2];         // 時刻[ms]

        int integrated_ = 0;    // 積分値(I制御の計算)
        bool usePid_ = false;   // I/D制御の情報が揃っているか

        int maxPwm_;            // PWMの最大値

        int lPwm_;
        int rPwm_;

        double  kp_;
        double  ki_;
        double  kd_;


        // Device
        device::Motors* motors_;
        ev3api::Clock clock_;

    public:

        PidController();

        /**
         * @brief ライントレースする際のPWM値(モーターを回転させる強さ≒ライントレースする速さ)をセットする
         *         片方のタイヤは常にこの強さで走行する
         * @param maxPwm
         * @author kuno
         */
         void setMaxPwm(int maxPwm = PID_DEFAULT_MAXPWM);

        /**
         * @brief PIDパラーメータをセットする
         * @param kp P制御の係数
         * @param ki I制御の係数
         * @param kd D制御の係数
         * @sa run
         */
        void setPid(double kp = PID_DEFAULT_KP, double ki = PID_DEFAULT_KI, double kd = PID_DEFAULT_KD);

        /**
         * @brief I以外のPIDパラーメータをセットする(ki = 0)
         * @param kp P制御の係数
         * @param kd D制御の係数
         * @sa run
         */
        void setPd(double kp = PID_DEFAULT_KP, double kd = PID_DEFAULT_KD);


        /**
         * @brief PID制御で走る
         * @details 左側に動きたい(右側にずれている)時ほど controllTargetを大きくし、右側に動きたい時ほど 小さく(負の数)にする
         * @param controllTarget PID制御での計算に利用するセンサなどの値(-500 ~500 程度)
         * @sa calculatePid
         */
        void run(int controllTarget);

        /**
         * @brief PID制御の内部の情報をリセットする
         * @details 積分の値、ひとつ前のセンサの値、時間の情報を初期化する。ライントレースを使うときには必ず使う直前に最初に呼ぶ
         **/
        void reset();

        /**
         * @brief PID制御の計算を行う
         * @details PID制御の入力値を与え、その値からPID制御の計算結果を返す
         * @param controllTarget PID制御で使うのセンサ等の値 (-500 ~ 500 程度の値になるようにする)
         *
         * @return 制御量の値 (-1 ~ 1 程度の値, 精度は0.001程度)
         */
        double calculatePid(int controllTarget);


    private:

        /**
         * @brief 車体の角速度からモータのPWMを計算する
         * @details deltaRad 角速度[rad / 内側のタイヤが進んだ距離] 左側に曲がるときが正の値, 最大2000程度
         *
         * @param deltaRad 角速度[rad / 内側のタイヤが進んだ距離] (正確にはなっていない)
         */
        void calculatePwm(int deltaRad);

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
