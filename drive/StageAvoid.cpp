#include "StageAvoid.h"

namespace drive{
	//コンストラクタ
	StageAvoid::StageAvoid(){	
		curveRunning_ = new CurveRunning();
		straightRunning_ = new StraightRunning();
		lineDetection_ = new detection::LineDetection();
		bodyAngle_ = new measurement::BodyAngleMeasurement();
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
		bodyAngle_->setBaceAngle();
		curveRunning_->run(10,50);
		while(1)
			if(!bodyAngle_->getResult() >= 45){
			straightRunning_->run(50);
			break;
	}
		while(1)
			if(!lineDetection_->getResult(20))
				break;
		curveRunning_->run(10,100);
	}
	void StageAvoid::left(){
		bodyAngle_->setBaceAngle();
	}
	void StageAvoid::straightRight(){
		bodyAngle_->setBaceAngle();
	}

	void StageAvoid::straightLeft(){
		bodyAngle_->setBaceAngle();
	}
}