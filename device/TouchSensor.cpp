#include "TouchSensor.h"

namespace device
{
    // インスタンス変数初期化
    TouchSensor* TouchSensor::_instance = 0;

    // シングルトン
    TouchSensor* TouchSensor::getInstance(){
        if(_instance == 0){
            _instance = new TouchSensor();
        }
        return _instance;
    }

    TouchSensor::TouchSensor() :
        // ポートの指定
        ev3api::TouchSensor(PORT_4)
    {}
}
