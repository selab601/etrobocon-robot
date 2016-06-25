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

Gyro::Gyro()
    // ポートの指定
    :gyroSensor(PORT_1)
{}

void Gyro::reset(){
    gyroSensor.reset();
}

void Gyro::setOffset(int16_t offset){
    this->offset = offset;
    gyroSensor.setOffset(offset);
}

int16_t Gyro::getAnglerVelocity(){
    return gyroSensor.getAnglerVelocity();
}

int16_t Gyro::getAngle(){
    return gyroSensor.getAngle();
}
