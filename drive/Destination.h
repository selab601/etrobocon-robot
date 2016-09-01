#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#include "Avoidance.h"
#include "StraightRunning.h"
#include "PivotTurn.h"
#include "BlockAreaCoordinate.h"
#include <cstdlib>

namespace drive{
    class Destination{
    public:
        /**
         * @brief EV3が向いている台座から見たEV3の位置(モデル2.3.4参照)
         **/
        enum class EV3Position{
            RIGHT,
            LEFT,
            UP,
            DOWN
        };
    private:
        /**
         * @brief EV3が向いている台座の座標(モデル2.3.4参照)
         **/
        BlockAreaCoordinate stageCoordinate_;
        EV3Position EV3Position_;
        Avoidance avoidance_;
        StraightRunning straightRunning_;
        PivotTurn pivotTurn_;

        BlockAreaCoordinate horizonal(int diffX);
        BlockAreaCoordinate vertical(int diffY);
    public:
        /**
         * @param 目的地台座の座標
         * @brief 目的地台座を与えると次に移動する台座の座標を返す
         * @return 次に移動する台座
         **/
        BlockAreaCoordinate getNextStageCoordinate(BlockAreaCoordinate destination);

        /**
         * @param x 現在、EV3が向いている台座のX座標
         * @param y 現在、EV3が向いている台座のy座標
         * @param Psition　EV3が向いている台座から見たEV3の位置
         **/
        Destination(int x, int y, Destination::EV3Position position);

        /**
         * @param x 目的地台座のx座標
         * @param y 目的地台座のy座標
         * @return 目的地台座の直前まで移動したらtrue
         **/
        bool runTo(int x,int y);
    };
};

#endif
