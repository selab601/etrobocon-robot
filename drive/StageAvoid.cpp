#include "StageAvoid.h"

namespace drive{
	//コンストラクタ
	StageAvoid::StageAvoid(){
		curveRunning_ = new CurveRunning();
		straightRunning_ = new StraightRunning();
		lineDetection_ = new detection::LineDetection();

		bodyAngle_ = new measurement::BodyAngleMeasurement();
		move_ = Move::START;
	}
	bool StageAvoid::startAvoidance(DirectionKind kind){
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
	bool StageAvoid::right(){
		switch(move_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			move_ = Move::TURN_RIGHT_1;
			break;
		case Move::TURN_RIGHT_1:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -45){
				move_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
                straightRunning_->run(0);
                ev3_speaker_play_tone ( 500, 100);
				move_ = Move::TURN_RIGHT_2;
			}
			break;
		case Move::TURN_RIGHT_2:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -90){
				move_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
        default :
            return false;
		}
        return false;
	}
	bool StageAvoid::left(){
		switch(move_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			move_ = Move::TURN_LEFT_1;
			break;
		case Move::TURN_LEFT_1:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 45){
				move_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				move_ = Move::TURN_LEFT_2;
			}
			break;
		case Move::TURN_LEFT_2:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 90){
				move_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
        default :
			return false;
		}
        return false;
	}
	bool StageAvoid::straightRight(){
		switch(move_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			move_ = Move::TURN_RIGHT_1;
			break;
		case Move::TURN_RIGHT_1:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -35){
				move_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				move_ = Move::TURN_LEFT_1;
			}
			break;
		case Move::TURN_LEFT_1:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 35){
				move_ = Move::STRAIGHT_2;
			}
			break;
		case Move::STRAIGHT_2:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				move_ = Move::TURN_RIGHT_2;
			}
			break;
		case Move::TURN_RIGHT_2:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= 0){
				move_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
            break;
        default:
			return false;
		}
        return false;
	}

	bool StageAvoid::straightLeft(){
		switch(move_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			move_ = Move::TURN_LEFT_1;
			break;
		case Move::TURN_LEFT_1:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() <= 35){
				move_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				move_ = Move::TURN_RIGHT_1;
			}
			break;
		case Move::TURN_RIGHT_1:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() >= -35){
				move_ = Move::STRAIGHT_2;
			}
			break;
		case Move::STRAIGHT_2:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				move_ = Move::TURN_LEFT_2;
			}
			break;
		case Move::TURN_LEFT_2:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() <= 0){
				move_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
        default:
			return false;
		}
        return false;
	}
}
