#include "straightRunning.h"

namespace drive{
	//メンバ
	straightRunning* straightRunning::instance = 0;

	//コンストラクタ
	straightRunning::straightRunning(){
		motor_	= Motors::getInstance();
	}

	//インスタンス取得
	straightRunning* straightRunning::getInstance(){
		if(instance == 0){
			instance = new straightRunning();
		}
		return instance;
	}

	void straightRunning::runStraight(int speed){
		motor_->setWheelPWM(speed,speed);
	}
}