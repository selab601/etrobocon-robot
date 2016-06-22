#ifndef TOUCH_
#define TOUCH_

#include "TouchSensor.h"
#include "SonarSensor.h"
#include "GyroSensor.h"

using namespace ev3api;

class Touch
{
public:
	static Touch* getInstance();
//タッチセンサ
	/**
	 * タッチセンサ値を返す
	 * 押されていたらtrue
	 * @return タッチセンサ値
	 */
	bool isPressed();

private:
	//オブジェクトへのポインタ定義
	TouchSensor    touchSensor;

	static Touch* _instance;
	//コンストラクタ
	Touch();

};

#endif