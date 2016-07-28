#include "DetectTableState.h"

using namespace detection;
using namespace device;

namespace drive {
    DetectTableState::DetectTableState() {
        colorDetection_ = new ColorDetection();
        lineTrace_ = LineTrace::getInstance();
        motors_ = Motors::getInstance();
        isTableDetected_ = false;
    }

    DetectTableState::~DetectTableState() {}

    bool DetectTableState::executed(colorset_t* result) {
        // TODO: 距離でセーフティをかける
        colorid_t color = colorDetection_->getResult();
        if (color == COLOR_RED
            || color == COLOR_BLUE
            || color == COLOR_GREEN
            || color == COLOR_YELLOW) {
            isTableDetected_ = true;
            detectedColor_ = color;
        }

        if (isTableDetected_) {
            // TODO: 色がうまく取れなかったときの補正
            result->tableColor = colorDetection_->getResult();
            motors_->setWheelPWM(0,0);
            return true;
        } else {
            // TODO: エッジやPIDの指定
            lineTrace_->run(20, LineTraceEdge::RIGHT);
        }
        return false;
    }
}
