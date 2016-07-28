#ifndef DETECT_TABLE_STATE_H_
#define DETECT_TABLE_STATE_H_

#include "IBlockColorCheckState.h"
#include "DetectBlockState.h"
#include "drive/LineTrace.h"
#include "device/Motors.h"
#include "detection/ColorDetection.h"

namespace drive {
    class DetectTableState : public IBlockColorCheckState {
    private:
        detection::ColorDetection* colorDetection_;
        drive::LineTrace* lineTrace_;
        device::Motors* motors_;
        bool isTableDetected_;
        colorid_t detectedColor_;

    public:
        DetectTableState();
        ~DetectTableState();
        bool executed(colorset_t* result);
    };
}

#endif
