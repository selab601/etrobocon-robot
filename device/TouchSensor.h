#ifndef TOUCHSENSOR_H_
#define TOUCHSENSOR_H_

#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/TouchSensor.h"

namespace device
{
    class TouchSensor : public ev3api::TouchSensor
    {
    private:
        static TouchSensor* _instance;

    private:
        TouchSensor();

    public:
        static TouchSensor* getInstance();
    };
}

#endif
