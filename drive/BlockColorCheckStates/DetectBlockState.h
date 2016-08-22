#ifndef DETECT_BLOCK_STATE_H_
#define DETECT_BLOCK_STATE_H_

#include "IBlockColorCheckState.h"
#include "DetectTableState.h"
#include "measurement/DistanceMeasurement.h"
#include "measurement/CountMeasurement.h"
#include "drive/StraightRunning.h"
#include "device/Motors.h"
#include "detection/ColorDetection.h"
#include "./BlockColorChecker.h"

namespace drive {
    class DetectBlockState : public IBlockColorCheckState {
    private:
        measurement::DistanceMeasurement* distanceMeasure_;
        measurement::CountMeasurement* countMeasure_;
        drive::StraightRunning* straight_;
        device::Motors* motors_;
        detection::ColorDetection* colorDetection_;
        bool initialized_;
        BlockColorChecker colorChecker_;
    public:
        DetectBlockState();
        ~DetectBlockState();
        bool isExecuted(colorset_t* result);
    };
}

#endif
