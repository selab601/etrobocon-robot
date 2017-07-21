#ifndef _AVOIDANCE_
#define _AVOIDANCE_

#include "measurement/BodyAngleMeasurement.h"
#include "PolarRunning.h"
#include "ev3api.h"

namespace drive{
    /**
     * @brief 台座回避走行クラス
     **/
    class Avoidance{
    private:
        enum class RunToState{
            INIT,
            POLAR,
            TURN,
            FINISHED
        };

        // runTo
        RunToState runToState_ = RunToState::INIT;
        int startDegree_;
        int polarMm_;
        int polarDegree_;
        PolarRunning polar_;
        measurement::BodyAngleMeasurement* bodyAngle_;
        bool hasBlock_ = false;

    public:

        /**
         * @brief  コンストラクタ
         */
        Avoidance();

        /**
         * @brief ブロック置き場を回避して他のラインに移動する
         * @details PolarRunningを利用して、他のラインに移動する
         *      ラインの真ん中に居て、ラインと平行になっている必要がある
         * @param currentMm 現在のラインの長さ[mm]
         * @param dstMMm 目的地ラインの長さ[mm]
         * @param degree 現在のラインからみた、目的地ラインの曲がった角度[degree]
         *
         * @return 終了したらtrue
         */
        bool runTo(int currentMm, int dstMMm, int degree);


        /**
         * @brief ブロックを持っているか知らせる
         * @details ブロックを持っている場合は離さないように超信地旋回を避ける
         *
         * @param hasBlock ブロックを持っているときtrue
         *
         */
        void hasBlock(bool hasBlock);

    };

}

#endif
