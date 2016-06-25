#ifndef TOUCHSENSOR_H_
#define TOUCHSENSOR_H_

#include "TouchSensor.h"

using namespace ev3api;

class Touch : ev3api::TouchSensor
{
private:
    static Touch* _instance;

private:
    Touch();

public:
    static Touch* getInstance();
};

#endif
