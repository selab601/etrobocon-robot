#include "CurveRunning.h"

namespace drive{
	//メンバ
	CurveRunning* CurveRunning::instance = 0;

	//コンストラクタ
	CurveRunning::CurveRunning(){
		motor_	= device::Motors::getInstance();
	}

	//インスタンス取得
	CurveRunning* CurveRunning::getInstance(){
		if(instance == 0){
			instance = new CurveRunning();
		}
		return instance;
	}

	void CurveRunning::run(int Rspeed,int Lspeed){
		motor_->setPWM(device::MOTOR_RIGHT,Rspeed);
		motor_->setPWM(device::MOTOR_LEFT,Lspeed);
	}
}