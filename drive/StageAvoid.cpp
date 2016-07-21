#include "StageAvoid.h"

namespace drive{
	//コンストラクタ
	StageAvoid::StageAvoid(){	
		curveRunning_ = new CurveRunning();
		straightRunning_ = new StraightRunning();
		lineDetection_ = new detection::LineDetection();
		
		bodyAngle_ = new measurement::BodyAngleMeasurement();
		MOVE_ = Move::START;
	}
	bool StageAvoid::startAvoidance(DirectionKind KIND){
		switch (KIND) {
		case DirectionKind::RIGHT:
			return(this->right());
		case DirectionKind::LEFT:
			return(this->left());
		case DirectionKind::STRAIGHT_RIGHT:
			return(this->straightRight());
		case DirectionKind::STRAIGHT_LEFT:
			return(this->straightLeft());
		}
	}
	bool StageAvoid::right(){
		switch(MOVE_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			MOVE_ = Move::TURN_RIGHT_1;
			break;
		case Move::TURN_RIGHT_1:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -45){
				MOVE_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				MOVE_ = Move::TURN_RIGHT_2;
			}
			break;
		case Move::TURN_RIGHT_2:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -90){
				MOVE_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
			return false;
		}
	}
	bool StageAvoid::left(){
		switch(MOVE_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			MOVE_ = Move::TURN_LEFT_1;
			break;
		case Move::TURN_LEFT_1:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 45){
				MOVE_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				MOVE_ = Move::TURN_LEFT_2;
			}
			break;
		case Move::TURN_LEFT_2:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 90){
				MOVE_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
			return false;
		}
	}
	bool StageAvoid::straightRight(){
		switch(MOVE_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			MOVE_ = Move::TURN_RIGHT_1;
			break;
		case Move::TURN_RIGHT_1:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -35){
				MOVE_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				MOVE_ = Move::TURN_LEFT_1;
			}
			break;
		case Move::TURN_LEFT_1:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 35){
				MOVE_ = Move::STRAIGHT_2;
			}
			break;
		case Move::STRAIGHT_2:
			straightRunning_->run(20);
			if(lineDetection2_->getResult(20)){
				MOVE_ = Move::TURN_RIGHT_2;
			}
			break;
		case Move::TURN_RIGHT_2:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= 0){
				MOVE_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
			return false;
		}
	}

	bool StageAvoid::straightLeft(){
		switch(MOVE_){
		case Move::START:
			bodyAngle_->setBaceAngle();
			MOVE_ = Move::TURN_LEFT_1;
			break;
		case Move::TURN_LEFT_1:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() <= 35){
				MOVE_ = Move::STRAIGHT_1;
			}
			break;
		case Move::STRAIGHT_1:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				MOVE_ = Move::TURN_RIGHT_1;
			}
			break;
		case Move::TURN_RIGHT_1:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() >= -35){
				MOVE_ = Move::STRAIGHT_2;
			}
			break;
		case Move::STRAIGHT_2:
			straightRunning_->run(20);
			if(lineDetection2_->getResult(20)){
				MOVE_ = Move::TURN_LEFT_2;
			}
			break;
		case Move::TURN_LEFT_2:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() <= 0){
				MOVE_ = Move::START;
				straightRunning_->run(0);
				return true;
			}
			return false;
		}
	}
}