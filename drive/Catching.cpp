#include "./Catching.h"

using namespace device;
using namespace measurement;
using namespace detection;
namespace drive{

    Catching::Catching(){
        this->destination_ = Destination::getInstance();
    }

    bool Catching::catchBlock(TurnDirection direction){
        if (TurnDirection::BACK == direction){
            return catchBackBlock();
        }

        SelfPositionEstimation* estimation = SelfPositionEstimation::getInstance();
        LineTrace* lineTrace = LineTrace::getInstance();
        static long baseLength = 0;
        static PivotTurn pivotTurn = PivotTurn();

        switch(state_){
            case State::INIT:
                lineTrace->setPid(0.0168, 0, 0.924);
                lineTrace->setMaxPwm(20);
                lineTrace->setEdge(LineTraceEdge::RIGHT);
                lineTrace->setTarget(0.2); // 黒寄り

                if (TurnDirection::STRAIGHT == direction){
                    state_ = State::TURN_RIGHT;
                }
                else{
                    state_ = State::TO_BLOCK;
                }
                break;

            case State::TURN_RIGHT:
                if (pivotTurn.turn(-20)){
                    state_ = State::TO_BLOCK;
                }
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

                    lineTrace->setPid(0.0168, 0, 0.924);
                    lineTrace->setMaxPwm(20);
                    lineTrace->setEdge(LineTraceEdge::RIGHT);
                    lineTrace->setTarget(0.2); // 黒寄り

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

    bool Catching::catchBlock(int x, int y){
        return catchBlock(BlockAreaCoordinate(x, y));
    }

    bool Catching::catchBlock(BlockAreaCoordinate destination){
        BlockAreaCoordinate nextCoordinate = destination_->getNextStageCoordinate(destination);
        Destination::Direction direction = destination_->getDirection(destination_->currentCoordinate_, nextCoordinate);
        Destination::Position position = destination_->getPosition(destination_->EV3Position_, direction);
        TurnDirection turnDirection = TurnDirection::LEFT;

        switch (position){
            case Destination::Position::EQUAL:
                turnDirection = TurnDirection::BACK;
                break;

            case Destination::Position::REVERSE:
                turnDirection = TurnDirection::STRAIGHT;
                break;

            case Destination::Position::RIGHT:
                turnDirection = TurnDirection::RIGHT;
                break;

            case Destination::Position::LEFT:
                turnDirection = TurnDirection::LEFT;
                break;

            case Destination::Position::NONE:
                return false;       // エラー
        }

        if (catchBlock(turnDirection)){
            destination_->update(nextCoordinate, position); // 情報の更新
            return true;
        }
        else{
            return false;
        }
    }

    bool Catching::catchBackBlock(){
        static ChangeDirectionState state = ChangeDirectionState::INIT;
        static Avoidance avoidance = Avoidance();
        static PivotTurn pivotTurn = PivotTurn();
        static DirectionKind directionKind = getAdvancableDirection();
        static TurnDirection turnDirection = DirectionKind::LEFT == directionKind? TurnDirection::RIGHT : TurnDirection::LEFT;
        static CurveRunning curveRunning = CurveRunning();
        switch (state){
            case ChangeDirectionState::INIT:
                directionKind = getAdvancableDirection();
                turnDirection = DirectionKind::LEFT == directionKind? TurnDirection::RIGHT : TurnDirection::LEFT;
                state = ChangeDirectionState::AVOIDANCE;
                break;

            case ChangeDirectionState::AVOIDANCE:

                if (avoidance.startAvoidance(directionKind)){
                    state = ChangeDirectionState::TURN;
                }
                break;

            case ChangeDirectionState::TURN:
                if (pivotTurn.turn(90)){
                    state = ChangeDirectionState::TURN_TO_LINE;
                }
                break;

            case ChangeDirectionState::TURN_TO_LINE:
                {
                    curveRunning.run(20, -20);
                    int black = device::ColorSensor::getInstance()->getBlackCalibratedValue();
                    int mid = black + 10;
                    if (device::ColorSensor::getInstance()->getBrightness() < mid){
                        curveRunning.run(0, 0);
                        state = ChangeDirectionState::CATCHING;
                    }
                }
                break;

            case ChangeDirectionState::CATCHING:
                if (catchBlock(turnDirection)){
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

    bool Catching::putBlock() {
        SelfPositionEstimation* estimation = SelfPositionEstimation::getInstance();
        LineTrace* lineTrace = LineTrace::getInstance();
        static long baseLength = 0;
        static State state = State::INIT;
        static CurveRunning curveRunning = CurveRunning();

        switch(state){
            case State::INIT:
                lineTrace->setPid(0.0168, 0, 0.924);
                lineTrace->setMaxPwm(20);
                lineTrace->setEdge(LineTraceEdge::RIGHT);
                lineTrace->setTarget(0.2); // 黒寄り

                state = State::TO_BLOCK;
                break;

            case State::TO_BLOCK:
                lineTrace->run();

                if (daizaDetected()){
                    state = State::TO_LINE;
                    baseLength = estimation->getMigrationLength();
                }
                break;

            case State::TO_LINE:
                {
                    long length = estimation->getMigrationLength() - baseLength;

                    curveRunning.run(-20, -20);
                    if(140 <= length){
                        ev3_speaker_play_tone(500,100);     // TODO: delete
                        state = State::FINISHED;
                    }
                }
                break;

            case State::FINISHED:
                stop();
                state = State::INIT;
                return true;
                break;

            case State::TURN:   //使わない
            case State::TURN_RIGHT:   //使わない
                break;
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
                    int pwm = diffDegree + 15;
                    pwm = pwm > 60? 60: pwm;
                    curveRunning.run(0,  pwm);
                }
                // 左に曲がるとき
                else {
                    int pwm = diffDegree + 15;
                    pwm = pwm > 60? 60: pwm;
                    curveRunning.run(pwm, pwm/10);
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
                return turn(80);

            case TurnDirection::STRAIGHT:
                return straight(145);

            case TurnDirection::BACK:   // 使わない
                break;
        }
        return false;
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
        static PivotTurn pivotTurn = PivotTurn();
        long currentLength = estimation->getMigrationLength() - baseLength;

        if (!initialized ){
            baseLength = estimation->getMigrationLength();
            if (pivotTurn.turn(-4)){    // TODO: 実験して角度を調整
                initialized = true;
            }
        }
        else{
            int pwm = (length - currentLength)/3 + 20;
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

    DirectionKind Catching::getAdvancableDirection(){
        switch(destination_->EV3Position_){
            case Destination::Direction::RIGHT:
                if (destination_->currentCoordinate_.getY() == 1){
                    return DirectionKind::LEFT;
                }
                break;

            case Destination::Direction::LEFT:
                if (destination_->currentCoordinate_.getY() == 4){
                    return DirectionKind::LEFT;
                }
                break;

            case Destination::Direction::UP:
                if (destination_->currentCoordinate_.getX() == 1){
                    return DirectionKind::LEFT;
                }
                break;

            case Destination::Direction::DOWN:
                if (destination_->currentCoordinate_.getX() == 4){
                    return DirectionKind::LEFT;
                }
                break;

            case Destination::Direction::NONE:
                // ここに来ることはないはずなのでデバッグ用
                ev3_speaker_play_tone(700, 3000);
                break;
        }
        return DirectionKind::RIGHT;
    }
}
