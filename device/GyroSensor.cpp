#include "GyroSensor.h"

namespace device
{
    // インスタンス変数初期化
    GyroSensor* GyroSensor::instance_ = 0;

    // シングルトン
    GyroSensor* GyroSensor::getInstance(){
        if(instance_ == 0){
            instance_ = new GyroSensor();
        }
        return instance_;
    }

    GyroSensor::GyroSensor() :
        // ポートの指定
        ev3api::GyroSensor(PORT_1)
    {}
}
