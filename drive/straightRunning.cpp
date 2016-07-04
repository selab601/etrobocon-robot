#include "StraightRunning.h"

namespace drive{
	//メンバ
	StraightRunning* StraightRunning::instance = 0;

	//コンストラクタ
	StraightRunning::StraightRunning(){
		motor_	= device::Motors::getInstance();
	}

	//インスタンス取得
	StraightRunning* StraightRunning::getInstance(){
		if(instance == 0){
			instance = new StraightRunning();
		}
		return instance;
	}

	void StraightRunning::run(int speed){
		motor_->setWheelPWM(speed,speed);
	}
}