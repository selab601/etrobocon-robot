#include "Gyro.h"

// インスタンス変数初期化
Gyro* Gyro::_instance = 0;

// シングルトン
Gyro* Gyro::getInstance(){
    if(_instance == 0){
        _instance = new Gyro();
    }
    return _instance;
}

Gyro::Gyro() :
    // ポートの指定
    GyroSensor(PORT_1)
{}
