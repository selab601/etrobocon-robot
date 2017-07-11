#ifndef TOUCHSENSOR_H_
#define TOUCHSENSOR_H_

#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/TouchSensor.h"
#include "measurement/timeMeasurement.h"

namespace device
{
    class TouchSensor : public ev3api::TouchSensor
    {
    private:
        static TouchSensor* instance_;

        bool hasPressed_ = false;
        measurement::TimeMeasurement sinceLastClicked_;

    private:
        TouchSensor();

    public:
        static TouchSensor* getInstance();

        /**
         * @brief クリックされた(押して離した)時trueを返す
         * @details 前回クリックされてから0.5秒経過後でないとクリック判定されない
         *
         * @return クリックされたか
         */
        bool isClicked();
    };
}

#endif
