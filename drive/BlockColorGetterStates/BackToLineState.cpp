#include "BackToLineState.h"

using namespace measurement;
using namespace device;
using namespace drive;

namespace drive {
    BackToLineState::BackToLineState() {
        states_.push(BACK_STATE::INIT_TIME);
        states_.push(BACK_STATE::SET_ARM);
        states_.push(BACK_STATE::INIT_DISTANCE);
        states_.push(BACK_STATE::FINISH);
        distanceMeasurement_ = new DistanceMeasurement();
        straight_ = new StraightRunning();
        initialized_ = false;
        baseCount_ = 0;
    }

    bool BackToLineState::isExecuted(colorset_t* result) {

        switch (states_.front()) {
        case BACK_STATE::INIT_TIME:
        {
            states_.pop();
            break;
        }
        case BACK_STATE::INIT_DISTANCE:
        {
            distanceMeasurement_->startMeasurement();
            distanceMeasurement_->setTargetDistance(100);
            states_.pop();
            break;
        }
        case BACK_STATE::SET_ARM:
        {
            if (Arm::getInstance()->normal()){
                states_.pop();
            }
            break;
        }
        case BACK_STATE::BACK:
        {
            straight_->run(-10);
            if (distanceMeasurement_->getResult()) {
                straight_->run(0);
                states_.pop();
            }
            break;
        }
        case BACK_STATE::FINISH:
        {
            return true;
        }
        }

        return false;
    };
}
