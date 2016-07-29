#include "DetectBlockState.h"

using namespace measurement;
using namespace device;
using namespace detection;

namespace drive {
    DetectBlockState::DetectBlockState() {
        distanceMeasure_ = new DistanceMeasurement();
        // TODO: どのくらい上げればよいか実験
        countMeasure_ = new CountMeasurement(MotorKind::ARM, 70);
        straight_ = new StraightRunning();
        motors_ = Motors::getInstance();
        colorDetection_ = new ColorDetection();
        initialized_ = false;
    }

    DetectBlockState::~DetectBlockState() {}

    bool DetectBlockState::executed(colorset_t* result) {
        if (initialized_ == false) {
            distanceMeasure_->startMeasurement();
            initialized_ = true;
        }

        // 後ろに少し下がる
        // TODO: どのくらい下がればよいか実験
        distanceMeasure_->setTargetDistance(47);
        if (distanceMeasure_->getResult() == false) {
            straight_->run(-10);
            return false;
        }
        straight_->run(0);

        // アームを上げて色を見る
        // TODO: 色がうまく取れなかった時．前後に動いてみる？
        if (countMeasure_->comparedWithTargetCount() == PlusOrMinus::PLUS) {
            motors_->setPWM(motor_kind::MOTOR_ARM, 10);
            return false;
        }
        motors_->setPWM(motor_kind::MOTOR_ARM, 0);

        // 色を読む
        // TODO: ダメだったときどうするか
        result->blockColor = colorDetection_->getResult();

        return true;
    }
}
