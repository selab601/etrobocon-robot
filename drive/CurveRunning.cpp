/*
 * @brief カーブ走行クラス
 */

#include "CurveRunning.h"

namespace drive{
	//コンストラクタ
	CurveRunning::CurveRunning(){
		motor_	= device::Motors::getInstance();
	}

	void CurveRunning::run(int Rspeed,int Lspeed){
		motor_->setPWM(device::MOTOR_RIGHT,Rspeed);
		motor_->setPWM(device::MOTOR_LEFT,Lspeed);
	}
}