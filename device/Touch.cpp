#include "Touch.h"

//インスタンス変数初期化
Touch* Touch::_instance = 0;

//シングルトン
Touch* Touch::getInstance(){
	if(_instance == 0){
		_instance = new Touch();
	}
	return _instance;
}

//タッチセンサの状態取得
bool Touch::isPressed(){
	return touchSensor.isPressed();
}

//コンストラクタ
//ポートを指定している
Touch::Touch()
	:touchSensor(PORT_4)
	{}
