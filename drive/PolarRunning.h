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
        State state_;
        State xyState_;
        bool isCenterPivot_ = true;
        int traceMaxPwm_;
        int maxPwm_ = 40;
        int isTurnInit_ = true;


    public:
        //コンストラクタ
        PolarRunning();

        /**
         * @brief 指定した極座標に移動する
         *
         * @param distance 行きたい場所までの距離[mm]
         * @param polarDegree 行きたい場所までの角度[Degree]
         * @param turnDegree 最初に旋回する時の角度[Degree] (省略可)
         *
         * @return 終了したらtrue
         */
        bool runTo(int distance, int polarDegree, int turnDegree);
        bool runTo(int distance, int polarDegree);

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
         * @brief 超信地旋回・信地旋回を行う
         *
         * @param degree 回転する角度[Degree]
         * @param speed 回転する早さ(PWM)
         *
         * @return 終了したらtrue
         */
        bool turn(int degree, int speed);

    private:

        /**
         * @brief 極座標の角度が指定した角度になるように移動する
         * @param degree 極座標の角度[degree]
         */
        void traceDegree(int degree);

        /**
         * @brief PID制御で早さを計算する
         *
         * @param distance 極座標の距離[mm]
         */
        void calculateMaxPwm(int distance);
    };

}

#endif
