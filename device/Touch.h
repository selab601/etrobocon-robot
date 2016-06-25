#ifndef TOUCHSENSOR_H_
#define TOUCHSENSOR_H_

#include "TouchSensor.h"

using namespace ev3api;

class Touch
{
private:
    //! デバイスオブジェクト
    TouchSensor touchSensor;

    static Touch* _instance;

private:
    Touch();

public:
    static Touch* getInstance();

    /**
     * @brief タッチセンサ値を返す
     * @return タッチセンサ値 (押されていたら true)
     */
    bool isPressed();
};

#endif
