#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#include "Avoidance.h"
#include "StraightRunning.h"
#include "PivotTurn.h"
#include "BlockAreaCoordinate.h"
#include "RegulateDistanceStage.h"
#include "../measurement/SelfPositionEstimation.h"
#include <cstdlib>

namespace drive{
    class Destination{
    public:
        /**
         * @brief EV3が向いている台座から見たEV3の位置(モデル2.3.4参照)
         **/
        enum class Direction{
            RIGHT,
            LEFT,
            UP,
            DOWN,
            NONE
        };
        const char* getStr(Direction direction);
        enum class Position {
            EQUAL,
            REVERSE,
            RIGHT,
            LEFT,
            NONE
        };
        const char* getStr(Position position);
        // from からみた to の向き(position) を返す
        Position getPosition(Direction from, Direction to);

        enum class AvoidanceMethod{
            RIGHT,
            LEFT,
            STRAIGHT_RIGHT,
            STRAIGHT_LEFT,
            TRUN
        };

        Direction EV3Position_;
        BlockAreaCoordinate currentCoordinate_;

                /*
         * @brief 2つの座標を比較して位置関係を返す
         *        from から見て to がどちら側にあるか？
         *        注意: from からみて、to がX軸、Y軸上になければ使えない
         * @return to のある向き。重なっていた場合は NONE が返る
         */
        Direction getDirection(BlockAreaCoordinate from, BlockAreaCoordinate to);
    private:
        /**
         * @brief EV3が向いている台座の座標(モデル2.3.4参照)
         **/
        BlockAreaCoordinate targetCoordinate_;
        BlockAreaCoordinate nextCoordinate_;
        Position goingPosition_;
        Avoidance avoidance_;
        StraightRunning straightRunning_;
        PivotTurn pivotTurn_;
        RegulateDistanceStage regulateDistanceStage_;
        measurement::SelfPositionEstimation* selfPositionEstimation_;
        bool isFinished_;
        //命名後で考える
        BlockAreaCoordinate horizonal(int diffX);
        BlockAreaCoordinate vertical(int diffY);

    private:
        // インスタンス変数
        static Destination* instance_;

        Destination();

    public:
        // シングルトンパターン
        static Destination* getInstance();

        /**
         * @param x 現在、EV3が向いている台座のX座標
         * @param y 現在、EV3が向いている台座のy座標
         * @param Psition EV3が向いている台座から見たEV3の位置
         **/
        void setCurrentLocation(int x, int y, Direction EV3Position);

        /**
         * @param nextCoordinate 次に進む台座の座標
         * @param goingPosition 次に進む台座の現在台座から見た位置
         * @brief EV3の現在地の更新を行う
         **/
        void update(BlockAreaCoordinate nextCoordinate , Position position);

        /**
         * @param 目的地台座の座標
         * @brief 目的地台座を与えると次に移動する台座の座標を返す
         * @return 次に移動する台座
         **/
        BlockAreaCoordinate getNextStageCoordinate(BlockAreaCoordinate destination);

        /**
         * @param x 目的地台座のx座標
         * @param y 目的地台座のy座標
         * @return 目的地台座の直前まで移動したらtrue
         **/
        bool runTo(int x,int y);

        const char* getEV3Position();
        const char* getGoingPosition();
        const char* getCurrentCoordinateString();
        const char* getNextCoordinateString();
        const char* getTargetCoordinateString();
    };
};

#endif
