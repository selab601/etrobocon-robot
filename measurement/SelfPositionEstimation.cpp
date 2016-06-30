#include "SelfPositionEstimation.h"

namespace measurement
{
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
	//	angle_rad = 0;
		angle = 0;
		deltaTime = 0;
	//	front_buf = 0;
		left_buf = 0;
		right_buf = 0;
		velocityNXT = 0;
		angularNXT = 0;
		angularVL = 0;
		angularVR = 0;
		migrationLength = 0;
		ml = 0;
	}

	//更新
	void SelfPositionEstimation::update(int32_t countL,int32_t countR){
		//増分時間の計算，左項のdeltaTimeは前回の呼び出し時間になっている
		deltaTime = clock.now() - deltaTime;

	//	各車輪の角速度計算
	//	angularVL = (countL - left_buf)*M_PI*2 / (ENCORDER*(GEAR2_TOOTH/GEAR1_TOOTH)*deltaTime);
	//	angularVR = (countR - right_buf)*M_PI*2 / (ENCORDER*(GEAR2_TOOTH/GEAR1_TOOTH)*deltaTime);

		angularVL = (double)(countL - left_buf)*M_PI*2 / (double)(ENCORDER_GEAR*deltaTime);
		angularVR = (double)(countR - right_buf)*M_PI*2 / (double)(ENCORDER_GEAR*deltaTime);

		//NXT本体の速度，回転速度計算
		velocityNXT = (WHEEL_RADIUS/2)*angularVL + (WHEEL_RADIUS/2)*angularVR;
		angularNXT =  ((double)WHEEL_RADIUS/(double)TREAD)*angularVR - ((double)WHEEL_RADIUS/(double)TREAD)*angularVL;

		//向きの計算
		angle += angularNXT * (double)deltaTime;
		//位置の計算
		location.add_xy(velocityNXT * cos(angle) * deltaTime,velocityNXT * sin(angle) * deltaTime);
		//移動距離の計算
		ml += (velocityNXT > 0 ? velocityNXT : (-1) * velocityNXT) *  (double)deltaTime;
		migrationLength += (int)ml;
		ml = ml - (int)ml;

	// #ifdef COMPLETE_RUN_MODE
	// 	//距離による補正
	// 	if(iniC != -1){
	// 		if(iniC == 1 && migrationLength > 2700){
	// 			angle = 0;
	// 			location.set_xy(migrationLength,0);
	// 			iniC++;
	// 		}
	// 	}
	// #endif

		//エンコーダ値バッファ更新
		left_buf = countL;
		right_buf = countR;
		//呼び出し現在時間保持
		deltaTime = clock.now();
	}
	//座標取得
	Coordinates* SelfPositionEstimation::getLocation(){
		return &location;
	}
	//x座標
	long SelfPositionEstimation::getLocatinoX(){
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
	//	angle_rad = 0;
		angle = 0;
	//	deltaTime = 0;
	//	front_buf = 0;
	//	left_buf = 0;
	//	right_buf = 0;
		velocityNXT = 0;
		angularNXT = 0;
		angularVL = 0;
		angularVR = 0;
	//	clock.reset();
		migrationLength = 0;
		ml = 0;
	}
}
