/*
 * @brief 直進走行クラス
 */

#include "StraightRunning.h"

namespace drive{
	
	//コンストラクタ
	StraightRunning::StraightRunning(){
		motor_	= device::Motors::getInstance();
	}

	void StraightRunning::run(int speed){
		motor_->setWheelPWM(speed,speed);
	}
}