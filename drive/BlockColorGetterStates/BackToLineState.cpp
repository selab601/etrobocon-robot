#include "BackToLineState.h"

using namespace measurement;
using namespace device;
using namespace drive;

namespace drive {
    BackToLineState::BackToLineState() {
        states_.push(BACK_STATE::INIT);
        states_.push(BACK_STATE::DOWN_ARM);
        states_.push(BACK_STATE::SET_ARM);
        states_.push(BACK_STATE::BACK);
        states_.push(BACK_STATE::FINISH);
        motors_ = Motors::getInstance();
        countMeasure_ = new CountMeasurement(MotorKind::ARM, -10);
        distanceMeasurement_ = new DistanceMeasurement();
        straight_ = new StraightRunning();
        initialized_ = false;
        baseCount_ = 0;
        timeMeasurement_ = new TimeMeasurement();
    }

    bool BackToLineState::isExecuted(colorset_t* result) {

        switch (states_.front()) {
        case BACK_STATE::INIT:
        {
            timeMeasurement_->setTargetTime(800);
            timeMeasurement_->setBaseTime();
            distanceMeasurement_->startMeasurement();
            distanceMeasurement_->setTargetDistance(100);
            states_.pop();
            break;
        }
        case BACK_STATE::DOWN_ARM:
        {
            motors_->setPWM(motor_kind::MOTOR_ARM, -40);
            if (timeMeasurement_->getResult()) {
                motors_->setPWM(motor_kind::MOTOR_ARM, 0);
                baseCount_ = motors_->getCount(device::MOTOR_ARM);
                states_.pop();
            }
            break;
        }
        case BACK_STATE::SET_ARM:
        {
            int currentCount = motors_->getCount(device::MOTOR_ARM);
            int pwm = BackToLineState::ANGLE - (currentCount - baseCount_);
            motors_->setPWM(device::MOTOR_ARM, pwm);
            if (BackToLineState::ANGLE == (currentCount - baseCount_)) {
                motors_->reset();
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
