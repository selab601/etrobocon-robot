#ifndef _POLAR_RUNNING_H_
#define _POLAR_RUNNING_H_

#include "measurement/BodyAngleMeasurement.h"
#include "measurement/SelfPositionEstimation.h"
#include "device/Motors.h"
#include "PidController.h"
#include <stdlib.h> // abs()

namespace drive{

    /**
     * @brief 台座回避走行クラス
     **/
    class PolarRunning{
    private:
        enum class State{
            INIT,
            TURN,
            TRACE,
            RUNTO,
            FINISHED
        };

        measurement::BodyAngleMeasurement bodyAngle_;
        measurement::SelfPositionEstimation* selfPositioin_;
        PidController degreeController_;
        PidController distanceController_;

        device::Motors* motors_;
        State state_ = State::INIT;
        State xyState_ = State::INIT;
        bool isCenterPivot_ = true;
        bool isInitialize_ = true;   // 最初に初期化するか(前回からの位置を指定したい場合false)
        int traceMaxPwm_;
        int maxPwm_ = 40;
        int isTurnInit_ = true;
        int isBack_ = false;

        int currentDegree10_ = 0;
        int turnSpeed_ = 0;

        int targetMm_ = 0;
        int targetDegree10_ = 0;


    public:
        //コンストラクタ
        PolarRunning();

        /**
         * @brief 指定した極座標に移動する
         *
         * @param distance 行きたい場所までの距離[mm]
         * @param polarDegree10 行きたい場所までの角度の10倍[0.1Degree]
         * @param turnDegree10 最初に旋回する時の角度の10倍[0.1Degree] (省略可)
         *
         * @return 終了したらtrue
         */
        bool runTo(int distance, int polarDegree10, int turnDegree10);
        bool runTo(int distance, int polarDegree10);

        /**
         * @brief 指定した座標に移動する
         *
         * @param xMm 行きたい場所の相対的なX座標[mm]
         * @param yMm 行きたい場所の相対的なY座標[mm]
         *
         * @return 終了したらtrue
         */
        bool runToXY(int xMm, int yMm);

        /**
         * @brief PWM値をセットする
         *
         * @param maxPwm PWM値の最大値
         */
        void setMaxPwm(int maxPwm);

        /**
         * @brief 回転する軸を指定する
         *
         * @param isCenterPivot 機体の中心:true, 内側のタイヤ:false
         */
        void centerPivot(bool isCenterPivot = true);

        /**
         * @brief 内部の状態をリセットする
         * @details runTo()を最後までやらずに終了してしまった場合、リセットする必要がある
         */
        void reset();

        /**
         * @brief 車体角度を監視して超信地旋回・信地旋回を行う
         *
         * @param degree10 回転する角度の10倍[0.1Degree]
         * @param speed 回転する早さ(PWM)
         *
         * @return 終了したらtrue
         */
        bool bodyTurn(int degree10, int speed);

        /**
         * @brief 極座標の角度を監視して信地旋回を行う
         *
         * @param degree10 回転する角度の10倍0.1[Degree]
         * @param speed 回転する早さ(PWM)
         *
         * @return 終了したらtrue
         */
        bool polarTurn(int degree10, int speed);

        void initialize(bool isInitialize = true);

        /**
         * @brief 残りの距離を返す
         *
         * @return  残りの距離[mm]
         */
        int getLeftMm();

        /**
         * @brief 残りの角度の10倍を返す
         *
         * @return 残りの角度の10倍[0.1Deg]
         */
        int getLeftDeg10();

        /**
         * @brief バックで移動するかセットする
         *
         * @param isBack バックで移動する時true
         */
        void back(bool isBack);
    private:

        /**
         * @brief 極座標の角度が指定した角度になるように移動する
         */
        void traceDegree();

        /**
         * @brief 超信地旋回・信地旋回を行う
         * @details 監視する角度はcurrentDegree10_ に指定する
         * @param degree10 回転する角度の10倍[0.1Degree]
         * @param speed 回転する早さ(PWM)
         *
         * @return 終了したらtrue
         */
        bool turn(int degree10, int speed);
        /**
         * @brief PID制御で早さを計算する
         *
         */
        void calculateMaxPwm();
    };

}

#endif
