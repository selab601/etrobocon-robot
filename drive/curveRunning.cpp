#include "curveRunning.h"

namespace drive{
	//メンバ
	curveRunning* curveRunning::instance = 0;

	//コンストラクタ
	curveRunning::curveRunning(){
		motor_	= Motors::getInstance();
	}

	//インスタンス取得
	curveRunning* curveRunning::getInstance(){
		if(instance == 0){
			instance = new curveRunning();
		}
		return instance;
	}

	void curveRunning::runCurve(int Rspeed,int Lspeed){
		motor_->setPWM(MOTOR_RIGHT,Rspeed);
		motor_->setPWM(MOTOR_LEFT,Lspeed);
	}
}