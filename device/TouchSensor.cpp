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
        ev3api::TouchSensor(PORT_4),
        sinceLastClicked_()
    {
        sinceLastClicked_.setBaseTime();
        sinceLastClicked_.setTargetTime(500);
    }

    bool TouchSensor::isClicked(){
        bool isPressed = ev3api::TouchSensor::isPressed();
        // クリックしたときで、前回のクリックから0.5秒経過したとき
        if (hasPressed_ && !isPressed && sinceLastClicked_.getResult()){
            hasPressed_ = isPressed;
            return true;
        }
        hasPressed_ = isPressed;
        return false;
    }
}
