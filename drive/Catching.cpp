#include "./Catching.h"

using namespace measurement;
namespace drive{

    Catching::Catching(){

    }

    bool Catching::catchBlock(){

        switch(state_){
            case State::INIT:
                state_ = State::TO_BLOCK;
                break;

            case State::TO_BLOCK:
                state_ = State::TURN;
                break;

            case State::TURN:
                if ( turn(90) ){
                    state_ = State::FINISHED;
                }
                break;

            case State::TO_LINE:
                state_ = State::FINISHED;
                break;

            case State::FINISHED:

                return true;
                break;
        }

        return false;
    }

    bool Catching::turn(int degree){
        static CurveRunning curveRunning = CurveRunning();
        static BodyAngleMeasurement bodyAngleMeasurement = BodyAngleMeasurement();
        int diffDegree = degree - bodyAngleMeasurement.getResult();

        switch(turnState_){
                case TurnState::INIT:
                    bodyAngleMeasurement.setBaseAngle();
                    turnState_ = TurnState::TURN;
                break;

            case TurnState::TURN:
                // 右にまがるとき
                if (0 > diffDegree ){
                    diffDegree *= -1;
                    curveRunning.run(diffDegree, 0);
                }
                // 左に曲がるとき
                else {
                    curveRunning.run(0, diffDegree);
                }

                if (diffDegree <= 1){
                    turnState_ = TurnState::FINISHED;
                }
                break;

            case TurnState::FINISHED:
                curveRunning.run(0, 0);
                turnState_ = TurnState::INIT;

                return true;
        }
        return false;
    }
}
