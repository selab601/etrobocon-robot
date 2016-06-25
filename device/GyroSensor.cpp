#include "GyroSensor.h"

namespace device
{
    // インスタンス変数初期化
    GyroSensor* GyroSensor::_instance = 0;

    // シングルトン
    GyroSensor* GyroSensor::getInstance(){
        if(_instance == 0){
            _instance = new GyroSensor();
        }
        return _instance;
    }

    GyroSensor::GyroSensor() :
        // ポートの指定
        ev3api::GyroSensor(PORT_1)
    {}
}
