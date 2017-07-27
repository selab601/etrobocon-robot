#include "BodyAngleMeasurement.h"

namespace measurement {


    BodyAngleMeasurement::BodyAngleMeasurement() {
        baseAngle_ = 0;
        selfPositionEstimation_ = SelfPositionEstimation::getInstance();
    }

    void BodyAngleMeasurement::setBaseAngle() {
        baseAngle_ = selfPositionEstimation_->getAngle10();
    }

    int BodyAngleMeasurement::getResult() {
        int currentAngle = selfPositionEstimation_->getAngle10();
        return (currentAngle - baseAngle_)/10;
    }

    int BodyAngleMeasurement::getRelative10() {
        int currentAngle = selfPositionEstimation_->getAngle10();
        return (currentAngle - baseAngle_);
    }
}
