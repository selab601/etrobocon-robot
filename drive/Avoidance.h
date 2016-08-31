#ifndef _AVOIDANCE_
#define _AVOIDANCE_

#include "detection/LineDetection.h"
#include "measurement/BodyAngleMeasurement.h"
#include "CurveRunning.h"
#include "StraightRunning.h"
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

    };

}

#endif
