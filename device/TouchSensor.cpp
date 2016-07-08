#include "TouchSensor.h"

namespace device
{
    // インスタンス変数初期化
    TouchSensor* TouchSensor::instance_ = 0;

    // シングルトン
    TouchSensor* TouchSensor::getInstance(){
        if(instance_ == 0){
            instance_ = new TouchSensor();
        }
        return instance_;
    }

    TouchSensor::TouchSensor() :
        // ポートの指定
        ev3api::TouchSensor(PORT_4)
    {}
}
