#ifndef _AVOIDANCE_
#define _AVOIDANCE_

#include "detection/LineDetection.h"
#include "measurement/BodyAngleMeasurement.h"
#include "CurveRunning.h"
#include "StraightRunning.h"
#include "PolarRunning.h"
#include "ev3api.h"

namespace drive{
    /**
     * @brief 台座の避け方の方向の種類
     **/
    enum class DirectionKind{
        RIGHT,
        LEFT,
        STRAIGHT_RIGHT,
        STRAIGHT_LEFT
    };

    enum class RunToState{
        INIT,
        POLAR,
        TURN,
        FINISHED
    };

    /**
     * @brief 台座回避走行クラス
     **/
    class Avoidance{
    private:
        /**
         * @brief 現在の走行状態
        **/
        enum class State{
            START,
            TURN_RIGHT,
            TURN_LEFT,
            STRAIGHT_1,
            STRAIGHT_2,
            LINE_RETURN
        };
        CurveRunning* curveRunning_;
        StraightRunning* straightRunning_;
        detection::LineDetection* lineDetection_;
        measurement::BodyAngleMeasurement* bodyAngle_;
        State state_;

        // runTo
        RunToState runToState_ = RunToState::INIT;
        int startDegree_;
        int polarMm_;
        int polarDegree_;
        PolarRunning polar_;

    public:
        //コンストラクタ
        Avoidance();

        /**
         * @brief 台座回避を実行する
         * @param 台座の避け方の方向の種類
         * @return 回避走行が終了したらtrueを返す
         **/
        bool startAvoidance(DirectionKind KIND);

        /**
         * @brief 右折しながら台座回避
         * @brief スタート位置はブロックか3cm以上離れていれば成功する
         * @brief 終了地点は前で実行するほど後ろで終了する
         * @return 回避走行が終了したらtrueを返す
         **/
        bool right();

         /**
         * @brief 左折しながら台座回避
         * @brief スタート位置はブロックか3cm以上離れていれば成功する
         * @brief 終了地点は前で実行するほど後ろで終了する
         * @return 回避走行が終了したらtrueを返す
         **/
        bool left();

         /**
         * @brief 直進、右から回り込んで台座回避
         * @brief スタート位置はブロックから3cm以上離れていれば成功する
         * @return 回避走行が終了したらtrueを返す
         **/
        bool straightRight();

         /**
         * @brief 直進、左から回り込んで台座回避
         * @brief スタート位置はブロックから3cm以上離れていれば成功する
         * @return 回避走行が終了したらtrueを返す
         **/
        bool straightLeft();


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
