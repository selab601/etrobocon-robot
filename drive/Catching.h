#ifndef CATCHING_H
#define CATCHING_H

#include "./PivotTurn.h"
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

            enum class ChangeDirectionState{
                INIT,
                AVOIDANCE,
                TURN,
                FINISHED,
            }

            enum class TurnState{
                INIT,
                TURN,
                FINISHED,
            };
            State state_ = State::INIT;
            TurnState turnState_ = TurnState::INIT;

            Destination* destination_;

        public:
            Catching(Destination* destination);

            enum class TurnDirection{
                RIGHT,
                LEFT,
                STRAIGHT,
                BACK,
            };

            /**
             * @brief ブロックを取得する
             *
             * @return 終了したらtrue
             */
            bool catchBlock(TurnDirection direction );

            bool catch(TurnDirection direction);

        private:
            bool turn(int degree);
            bool turn(TurnDirection direction);
            bool straight(int length);

            bool daizaDetected();
            void stop();
    };
}
#endif
