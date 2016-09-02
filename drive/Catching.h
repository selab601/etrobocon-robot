#ifndef CATCHING_H
#define CATCHING_H

#include "./PivotTurn.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "./CurveRunning.h"
#include <stdlib.h>

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

            enum class TurnState{
                INIT,
                TURN,
                FINISHED,
            };
            State state_ = State::INIT;
            TurnState turnState_ = TurnState::INIT;

        public:
            Catching();


            /**
             * @brief ブロックを取得する
             *
             * @return 終了したらtrue
             */
            bool catchBlock();

        private:
            bool turn(int degree);
    };
}
#endif
