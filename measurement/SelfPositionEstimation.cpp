#include "SelfPositionEstimation.h"

namespace measurement
{
	using namespace ev3api;

	SelfPositionEstimation* SelfPositionEstimation::_instance = 0;

	SelfPositionEstimation* SelfPositionEstimation::getInstance(){
		if(_instance == 0){
			_instance = new SelfPositionEstimation();
		}
		return _instance;
	}

	SelfPositionEstimation::SelfPositionEstimation(){
		location = Coordinates();
		measurePoint = Coordinates();
		clock = Clock();
		angle = 0;
		deltaTime = 0;
		leftBuf = 0;
		rightBuf = 0;
		velocityEV3 = 0;
		angularEV3 = 0;
		angularVL = 0;
		angularVR = 0;
		migrationLength = 0;
		ml = 0;
	}

	//更新
	void SelfPositionEstimation::update(int32_t countL,int32_t countR){
		//増分時間の計算，左項のdeltaTimeは前回の呼び出し時間になっている
		deltaTime = clock.now() - deltaTime;

		angularVL = (double)(countL - leftBuf)*M_PI*2 / (double)(ENCORDER_GEAR*deltaTime);
		angularVR = (double)(countR - rightBuf)*M_PI*2 / (double)(ENCORDER_GEAR*deltaTime);

		//EV3本体の速度，回転速度計算
		velocityEV3 = (WHEEL_RADIUS/2)*angularVL + (WHEEL_RADIUS/2)*angularVR;
		angularEV3 =  ((double)WHEEL_RADIUS/(double)TREAD)*angularVR - ((double)WHEEL_RADIUS/(double)TREAD)*angularVL;

		//向きの計算
		angle += angularEV3 * (double)deltaTime;
		//位置の計算
		location.add_xy(velocityEV3 * cos(angle) * deltaTime,velocityEV3 * sin(angle) * deltaTime);
		//移動距離の計算
		ml += (velocityEV3 > 0 ? velocityEV3 : (-1) * velocityEV3) *  (double)deltaTime;
		migrationLength += (int)ml;
		ml = ml - (int)ml;

		//エンコーダ値バッファ更新
		leftBuf = countL;
		rightBuf = countR;
		//呼び出し現在時間保持
		deltaTime = clock.now();
	}
	//座標取得
	Coordinates* SelfPositionEstimation::getLocation(){
		return &location;
	}
	//x座標
	long SelfPositionEstimation::getLocationX(){
		return location.get_x();
	}
	//y座標
	long SelfPositionEstimation::getLocationY(){
		return location.get_y();
	}
	//計測開始
	void SelfPositionEstimation::startMeasure(){
		measurePoint.set_x(location.get_x());
		measurePoint.set_y(location.get_y());
	}
	//計測地点を原点としたx座標を返す
	long SelfPositionEstimation::getMeasureX(){
		return location.get_x() - measurePoint.get_x();
	}
	//計測地点を原点としたy座標を返す
	long SelfPositionEstimation::getMeasureY(){
		return location.get_y() - measurePoint.get_y();
	}
	//locationからの距離を返す
	int SelfPositionEstimation::getDistance(){
		return (int)(sqrt(location.get_x()*location.get_x() + location.get_y()*location.get_y()));
	}
	//移動距離を返す
	long SelfPositionEstimation::getMigrationLength(){
		return migrationLength;
	}
	//走行体の向いている方向を角度で返す
	int SelfPositionEstimation::getAngle(){
		return (int)(angle*180/M_PI);
	}
	//自己位置初期化
	void SelfPositionEstimation::initMap(){
		location.init();
		measurePoint.init();
		angle = 0;
		velocityEV3 = 0;
		angularEV3 = 0;
		angularVL = 0;
		angularVR = 0;
		migrationLength = 0;
		ml = 0;
	}
}
