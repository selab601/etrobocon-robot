#include "DetectBlockState.h"

using namespace measurement;
using namespace device;
using namespace detection;

namespace drive {
    DetectBlockState::DetectBlockState() {
        distanceMeasure_ = new DistanceMeasurement();
        straight_ = new StraightRunning();
        colorDetection_ = new ColorDetection();
        initialized_ = false;
        colorChecker_ = BlockColorChecker();
    }

    DetectBlockState::~DetectBlockState() {}

    bool DetectBlockState::isExecuted(colorset_t* result) {
        if (initialized_ == false) {
            distanceMeasure_->start();
            initialized_ = true;
        }

        // 後ろに少し下がる
        // TODO: どのくらい下がればよいか実験
        distanceMeasure_->setTarget(47);
        if (distanceMeasure_->getResult() == false) {
            straight_->run(-10);
            return false;
        }
        straight_->run(0);

        // アームを上げて色を見る
        // TODO: どのくらい上げればよいか実験
        if (!Arm::getInstance()->setDegree(70)) {
            return false;
        }

        // 色を読む
        colorid_t* tmpColor = new colorid_t;
        if (colorChecker_.isExecuted(tmpColor) == false) {
            return false;
        }
        result->blockColor = *tmpColor;

        return true;
    }
}
