#ifndef CATCHING_H
#define CATCHING_H

#include "./PivotTurn.h"
#include "./Destination.h"
#include "./Avoidance.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "./CurveRunning.h"
#include "./LineTrace.h"
#include "../detection/ColorDetection.h"
#include <stdlib.h>

#define TO_LINE_LENGTH 165  // 6センチになる！
namespace drive
{
    class Catching{
        private:

            enum class State
            {
                INIT,
                TO_BLOCK,
                TURN,
                TO_LINE,
                FINISHED,
            };

            // 方向を変えてから取得するときの状態
            enum class ChangeDirectionState{
                INIT,
                AVOIDANCE,
                TURN,
                CATCHING,
                FINISHED,
            };

            enum class TurnState{
                INIT,
                TURN,
                FINISHED,
            };
            State state_ = State::INIT;
            TurnState turnState_ = TurnState::INIT;

            Destination* destination_;

        public:
            Catching();

            enum class TurnDirection{
                RIGHT,
                LEFT,
                STRAIGHT,
                BACK,
            };

            /**
             * @brief ブロックを取得する
             *
             * @param x 目的地のx座標
             * @param y 目的地のy座標
             *
             * @return 終了したらtrue
             */
            bool catchBlock(int x, int y);


            /**
             * @brief ブロックを取得する
             *
             * @param destination 目的地の座標
             *
             * @return 終了したらtrue
             */
            bool catchBlock(BlockAreaCoordinate destination);

            /**
             * @brief ブロックを取得する
             *
             * @param direction 取得してから進む方向
             *
             * @return 終了したらtrue
             */
            bool catchBlock(TurnDirection direction );

        private:
            bool catchBackBlock();
            bool turn(int degree);
            bool turn(TurnDirection direction);
            bool straight(int length);
            DirectionKind getAdvancableDirection();

            bool daizaDetected();
            void stop();
    };
}
#endif
