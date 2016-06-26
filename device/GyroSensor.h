#ifndef GYROSENSOR_H_
#define GYROSENSOR_H_

#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/GyroSensor.h"

namespace device
{
    class GyroSensor : public ev3api::GyroSensor
    {
    private:
        static GyroSensor* _instance;

    private:
        GyroSensor();

    public:
        static GyroSensor* getInstance();
    };
}

#endif
