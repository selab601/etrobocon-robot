#include "Avoidance.h"

namespace drive{
	//コンストラクタ
	Avoidance::Avoidance(){
		curveRunning_ = new CurveRunning();
		straightRunning_ = new StraightRunning();
		lineDetection_ = new detection::LineDetection();

		bodyAngle_ = new measurement::BodyAngleMeasurement();
		state_ = State::START;
	}
	bool Avoidance::startAvoidance(DirectionKind kind){
		switch (kind) {
		case DirectionKind::RIGHT:
			return(this->right());
		case DirectionKind::LEFT:
			return(this->left());
		case DirectionKind::STRAIGHT_RIGHT:
			return(this->straightRight());
		case DirectionKind::STRAIGHT_LEFT:
			return(this->straightLeft());
        default:
            return false;
		}
	}
	bool Avoidance::right(){
		switch(state_){
		case State::START:
			bodyAngle_->setBaseAngle();
			state_ = State::TURN_RIGHT;
			break;
		case State::TURN_RIGHT:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -45){
				state_ = State::STRAIGHT_1;
			}
			break;
		case State::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
                straightRunning_->run(0);
				state_ = State::LINE_RETURN;
			}
			break;
		case State::LINE_RETURN:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -90){
				state_ = State::START;
				straightRunning_->run(0);
				return true;
			}
            break;
        default :
            return false;
		}
        return false;
	}
	bool Avoidance::left(){
		switch(state_){
		case State::START:
			bodyAngle_->setBaseAngle();
			state_ = State::TURN_LEFT;
			break;
		case State::TURN_LEFT:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 45){
				state_ = State::STRAIGHT_1;
			}
			break;
		case State::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				state_ = State::LINE_RETURN;
			}
			break;
		case State::LINE_RETURN:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 90){
				state_ = State::START;
				straightRunning_->run(0);
				return true;
			}
            break;
        default:
			return false;
		}
        return false;
	}
	bool Avoidance::straightRight(){
		switch(state_){
		case State::START:
			bodyAngle_->setBaseAngle();
			state_ = State::TURN_RIGHT;
			break;
		case State::TURN_RIGHT:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -40){
				state_ = State::STRAIGHT_1;
			}
			break;
		case State::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				state_ = State::TURN_LEFT;
			}
			break;
		case State::TURN_LEFT:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 40){
				state_ = State::STRAIGHT_2;
			}
			break;
		case State::STRAIGHT_2:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				state_ = State::LINE_RETURN;
			}
			break;
		case State::LINE_RETURN:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= 0){
				state_ = State::START;
				straightRunning_->run(0);
				return true;
			}
            break;
        default:
			return false;
		}
        return false;
	}

	bool Avoidance::straightLeft(){
		switch(state_){
		case State::START:
			bodyAngle_->setBaseAngle();
			state_ = State::TURN_LEFT;
			break;
		case State::TURN_LEFT:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() <= 35){
				state_ = State::STRAIGHT_1;
			}
			break;
		case State::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				state_ = State::TURN_RIGHT;
			}
			break;
		case State::TURN_RIGHT:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() >= -35){
				state_ = State::STRAIGHT_2;
			}
			break;
		case State::STRAIGHT_2:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				state_ = State::LINE_RETURN;
			}
			break;
		case State::LINE_RETURN:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() <= 0){
				state_ = State::START;
				straightRunning_->run(0);
				return true;
			}
            break;
        default:
			return false;
		}
        return false;
	}
}
