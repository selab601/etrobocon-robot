#ifndef GYROSENSOR_H_
#define GYROSENSOR_H_

#include "GyroSensor.h"

using namespace ev3api;

class Gyro : ev3api::GyroSensor
{
private:
    static Gyro* _instance;

private:
    Gyro();

public:
    static Gyro* getInstance();
};

#endif
