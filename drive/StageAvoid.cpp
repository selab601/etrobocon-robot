#include "StageAvoid.h"

namespace drive{
	//コンストラクタ
	StageAvoid::StageAvoid(){	
		curveRunning_ = new CurveRunning();
		straightRunning_ = new StraightRunning();
		lineDetection_ = new detection::LineDetection();
		bodyAngle_ = new measurement::BodyAngleMeasurement();
		TURN_ = Turn::START;
	}
	void StageAvoid::startAvoidance(DirectionKind KIND){
		switch (KIND) {
		case DirectionKind::RIGHT:
			this->right();
			break;
		case DirectionKind::LEFT:
			this->left();
			break;
		case DirectionKind::STRAIGHT_RIGHT:
			this->straightRight();
			break;
		case DirectionKind::STRAIGHT_LEFT:
			this->straightLeft();
		}
	}
	void StageAvoid::right(){
		switch(TURN_){
		case Turn::START:
			bodyAngle_->setBaceAngle();
			TURN_ = Turn::TURN_RIGHT_1;
			break;
		case Turn::TURN_RIGHT_1:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -45){
				TURN_ = Turn::STRAIGHT;
			}
			break;
		case Turn::STRAIGHT:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				TURN_ = Turn::TURN_RIGHT_2;
			}
			break;
		case Turn::TURN_RIGHT_2:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -90){
				TURN_ = Turn::STOP;
				straightRunning_->run(0);
				return;
			}
		}
	}
	void StageAvoid::left(){
		switch(TURN_){
		case Turn::START:
			bodyAngle_->setBaceAngle();
			TURN_ = Turn::TURN_LEFT_1;
			break;
		case Turn::TURN_LEFT_1:
			curveRunning_->run(30,5);
			if(bodyAngle_->getResult() >= 45){
				TURN_ = Turn::STRAIGHT;
			}
			break;
		case Turn::STRAIGHT:
			straightRunning_->run(20);
			if(lineDetection_->getResult(20)){
				TURN_ = Turn::TURN_LEFT_2;
			}
			break;
		case Turn::TURN_LEFT_2:
			curveRunning_->run(5,30);
			if(bodyAngle_->getResult() <= -90){
				TURN_ = Turn::STOP;
				straightRunning_->run(0);
				return;
			}
		}
	}
	void StageAvoid::straightRight(){
		bodyAngle_->setBaceAngle();
		curveRunning_->run(10,50);
		while(1)
			if(bodyAngle_->getResult() >= 45){
			straightRunning_->run(50);
			break;
			}
		while(1)
			if(lineDetection_->getResult(20)){
				curveRunning_->run(50,10);
				break;
			}
		while(1)
			if(bodyAngle_->getResult() <= -45){
				straightRunning_->run(50);
				break;
			}
		while(1)
			if(lineDetection_->getResult(20)){
				curveRunning_->run(10,50);
				break;
			}
		while(1)
			if(bodyAngle_->getResult() >= 0){
				straightRunning_->run(0);
				break;
			}
	}

	void StageAvoid::straightLeft(){
		bodyAngle_->setBaceAngle();
		curveRunning_->run(50,10);
		while(1)
			if(!bodyAngle_->getResult() <= -45){
			straightRunning_->run(50);
			break;
			}
		while(1)
			if(!lineDetection_->getResult(20)){
				curveRunning_->run(10,50);
				break;
			}
		while(1)
			if(!bodyAngle_->getResult() >= 45){
				straightRunning_->run(50);
				break;
			}
		while(1)
			if(!lineDetection_->getResult(20)){
				curveRunning_->run(50,10);
				break;
			}
		while(1)
			if(!bodyAngle_->getResult() <= 0){
				straightRunning_->run(0);
				break;
			}
	}
}