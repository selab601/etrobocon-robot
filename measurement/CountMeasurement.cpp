#include "CountMeasurement.h"

namespace measurement{


    CountMeasurement::CountMeasurement(MotorKind KIND, int32_t targetCount, int32_t baseCount){
        motor         = device::Motors::getInstance();
        targetCount_ = targetCount;
        KIND_         = KIND;
        this->setBaseCount(baseCount);
    }

    bool CountMeasurement::getResult(){
        int32_t nowCount = -1;
        switch (KIND_) {
        case MotorKind::ARM:
            nowCount = motor->getCount(device::MOTOR_ARM);
            break;
        case MotorKind::LEFT:
            nowCount = motor->getCount(device::MOTOR_LEFT);
            break;
        case MotorKind::RIGHT:
            nowCount = motor->getCount(device::MOTOR_RIGHT);
            break;
        case MotorKind::TAIL:
            nowCount = motor->getCount(device::MOTOR_TAIL);
        }
        if (targetCount_+baseCount_ == nowCount){ return true; }
        return false;
    }

    void CountMeasurement::setBaseCount(int32_t baseCount){
        if (baseCount == -1){
            switch (KIND_) {
                case MotorKind::ARM:
	                baseCount_ = motor->getCount(device::MOTOR_ARM);
	                break;
                case MotorKind::LEFT:
	                baseCount_ = motor->getCount(device::MOTOR_LEFT);
                    break;
                case MotorKind::RIGHT:
                    baseCount_ = motor->getCount(device::MOTOR_RIGHT);
                    break;
                case MotorKind::TAIL:
                    baseCount_ = motor->getCount(device::MOTOR_TAIL);
            }
        } else {
            baseCount_ = baseCount;
        }
    }

    void CountMeasurement::setTargetCount(int32_t targetCount){
        targetCount_ = targetCount;
    }

    void CountMeasurement::setCountConfig(int32_t targetCount, int32_t baseCount){
        this->setBaseCount(baseCount);
        this->setTargetCount(targetCount);
    }

    void CountMeasurement::setTargetMotor(MotorKind KIND){
        KIND_ = KIND;
    }

    measurement::PlusOrMinus CountMeasurement::comparedWithTargetCount(){
        int32_t nowCount = -1;
        switch (KIND_) {
            case MotorKind::ARM:
                nowCount = motor->getCount(device::MOTOR_ARM);
                break;
            case MotorKind::LEFT:
                nowCount = motor->getCount(device::MOTOR_LEFT);
                break;
            case MotorKind::RIGHT:
                nowCount = motor->getCount(device::MOTOR_RIGHT);
                break;
            case MotorKind::TAIL:
                nowCount = motor->getCount(device::MOTOR_TAIL);
            }

        if (targetCount_+baseCount_ > nowCount){
            return PlusOrMinus::PLUS;
        }
        return PlusOrMinus::MINUS;
    }
};
