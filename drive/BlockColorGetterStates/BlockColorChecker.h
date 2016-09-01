#ifndef BLOCK_COLOR_CHECKER
#define BLOCK_COLOR_CHECKER

#include "device/ColorSensor.h"
#include "device/Display.h"
#include "detection/ColorDetection.h"
#include "measurement/TimeMeasurement.h"
#include <ev3api.h>
#include <vector>
#include <queue>

namespace drive {
    class BlockColorChecker {
    private:
        enum class CHECKER_STATE {
            RGB,
            FINISH
        };
        std::queue<CHECKER_STATE> states_;
        std::vector<uint16_t> pastRValues_;
        std::vector<uint16_t> pastGValues_;
        std::vector<uint16_t> pastBValues_;
        std::vector<int> brightnessValues_;
        bool isInitializedTime_;
        device::ColorSensor* colorSensor_;
        detection::ColorDetection colorDetection_;
        measurement::TimeMeasurement timer_;
        device::Display* display_;

    public:
        BlockColorChecker();

        bool isExecuted(colorid_t* result);

    private:
        template <class T>
        T calcAverage(const std::vector<T> &vec, T start);
    };
}

#endif










