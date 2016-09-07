#include "./Catching.h"
#include "../device/Display.h"

using namespace device;
using namespace measurement;
using namespace detection;
namespace drive{

    Catching::Catching(Destination* destination){
        this->destination_ = destination;
    }

    bool Catching::catchBlock(TurnDirection direction){
        if (TurnDirection::BACK == direction){
            return catchBackBlock();
        }

        static ColorDetection colorDetection = ColorDetection();
        SelfPositionEstimation* estimation = SelfPositionEstimation::getInstance();
        LineTrace* lineTrace = LineTrace::getInstance();
        static long baseLength = 0;

        switch(state_){
            case State::INIT:
                lineTrace->setPid();
                lineTrace->setMaxPwm(20);
                lineTrace->setEdge(LineTraceEdge::RIGHT);
                lineTrace->setTarget(0.42); // 黒寄り

                state_ = State::TO_BLOCK;
                break;

            case State::TO_BLOCK:
                lineTrace->run();

                if (daizaDetected()){
                    state_ = State::TURN;
                }
                break;

            case State::TURN:
                if ( turn(direction) ){
                    baseLength = estimation->getMigrationLength();


                    lineTrace->setPid();
                    lineTrace->setMaxPwm(20);
                    lineTrace->setEdge(LineTraceEdge::RIGHT);
                    lineTrace->setTarget(0.42); // 黒寄り

                    state_ = State::TO_LINE;
                }
                break;

            case State::TO_LINE:
                {
                    long length = estimation->getMigrationLength() - baseLength;

                    lineTrace->run();
                    if(TO_LINE_LENGTH <= length){
                        ev3_speaker_play_tone(500,100);     // TODO: delete
                        state_ = State::FINISHED;
                    }
                }
                break;

            case State::FINISHED:
                stop();
                state_ = State::INIT;
                return true;
                break;
        }

        return false;
    }


    bool Catching::catchBackBlock(){
        static ChangeDirectionState state = ChangeDirectionState::INIT;
        static Avoidance avoidance = Avoidance();
        static PivotTurn pivotTurn = PivotTurn();

        switch (state){
            case ChangeDirectionState::INIT:
                state = ChangeDirectionState::AVOIDANCE;
                break;

            case ChangeDirectionState::AVOIDANCE:

                if (avoidance.right()){
                    state = ChangeDirectionState::TURN;
                }
                break;

            case ChangeDirectionState::TURN:
                if (pivotTurn.turn(180)){
                    state = ChangeDirectionState::CATCHING;
                }
                break;

            case ChangeDirectionState::CATCHING:
                if (catchBlock(TurnDirection::LEFT)){
                    state = ChangeDirectionState::FINISHED;
                }
                break;

            case ChangeDirectionState::FINISHED:
                stop();
                state = ChangeDirectionState::INIT;

                return true;
        }

        return false;
    }

    bool Catching::turn(int degree){
        static CurveRunning curveRunning = CurveRunning();
        static BodyAngleMeasurement bodyAngleMeasurement = BodyAngleMeasurement();
        int diffDegree = degree - bodyAngleMeasurement.getResult();
        if (diffDegree >= 100){
            diffDegree = 100;
        }

        switch(turnState_){
                case TurnState::INIT:
                    bodyAngleMeasurement.setBaseAngle();
                    turnState_ = TurnState::TURN;
                break;

            case TurnState::TURN:
                // 右にまがるとき
                if (0 > diffDegree ){
                    diffDegree *= -1;
                    curveRunning.run(0, diffDegree);
                }
                // 左に曲がるとき
                else {
                    curveRunning.run(diffDegree, diffDegree/10);
                }

                if (diffDegree <= 1){
                    turnState_ = TurnState::FINISHED;
                }
                break;

            case TurnState::FINISHED:
                stop();
                turnState_ = TurnState::INIT;

                return true;
        }
        return false;
    }

    bool Catching::turn(TurnDirection direction){
        switch(direction){
            case TurnDirection::RIGHT:
                return turn(-90);

            case TurnDirection::LEFT:
                return turn(90);

            case TurnDirection::STRAIGHT:
                return straight(145);
        }
    }

    void Catching::stop(){
        static CurveRunning curveRunning = CurveRunning();
        curveRunning.run(0, 0);
    }

    bool Catching::straight(int length){
        static CurveRunning curveRunning = CurveRunning();
        SelfPositionEstimation* estimation = SelfPositionEstimation::getInstance();
        static long baseLength = 0;
        static bool initialized = false;
        long currentLength = estimation->getMigrationLength() - baseLength;

        if (!initialized ){
            baseLength = estimation->getMigrationLength();
            initialized = true;
        }
        else{
            int pwm = (length - currentLength)/3 + 5;
            curveRunning.run(pwm, pwm);

            if(currentLength >= length){
                stop();
                initialized = false;
                return true;
            }
        }
        return false;
    }

    bool Catching::daizaDetected(){
        static ColorDetection colorDetection = ColorDetection();
        colorid_t colorId = colorDetection.getResult();

        switch(colorId){
            case COLOR_NONE:
            case COLOR_BLACK:
            case COLOR_WHITE:

            case COLOR_BROWN:
            case TNUM_COLOR:
                return false;

            case COLOR_RED:
            case COLOR_YELLOW:
            case COLOR_GREEN:
            case COLOR_BLUE:
                return true;
        }

        return false;
    }
}
