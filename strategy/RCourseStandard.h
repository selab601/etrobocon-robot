#ifndef _RCOURSE_STANDARD_H
#define _RCOURSE_STANDARD_H

// 1: ショートカットする, 0: しない
#define RCOURSE_SHORTCUT_FLAG   1   // TODO: ショートカットするか選択

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include "device/Motors.h"
#include "measurement/BodyAngleMeasurement.h"
#include "../measurement/SelfPositionEstimation.h"
#include "drive/CurveRunning.h"
#include "drive/StraightRunning.h"
#include "detection/LineDetection.h"
#include "drive/PivotTurn.h"

namespace strategy{
    class RCourseStandard : public IStrategy{
        private:
            enum class Status
            {
                DETECTION_STANDBY,
                DETECTION_CURVE,
                STRAIGHT1_STANDBY,
                STRAIGHT1,
                CURVE1_STANDBY,
                CURVE1,
                STRAIGHT2_STANDBY,
                STRAIGHT2,
                CURVE2_STANDBY,
                CURVE2,
                STRAIGHT3_STANDBY,
                STRAIGHT3,
                CURVE3_STANDBY,
                CURVE3,

                // ショートカットVer
                TO_NEAR_GATE_LINE_CURVE,
                TO_NEAR_GATE_LINE_STRAIGHT,
                LINE_DETECT,

                TO_GATE,
                TURN,

                STRAIGHT4_STANDBY,
                STRAIGHT4,
                DONE
            };
            Status Status_;

            drive::LineTrace* linetrace_;
            measurement::DistanceMeasurement distanceMeasurement_;
            device::Motors* motor_;
            measurement::BodyAngleMeasurement bodyAngleMeasurement_;
            drive::CurveRunning curveRunning_;
            drive::StraightRunning straightRunning_;
            detection::LineDetection lineDetection_;
            drive::PivotTurn pivotTurn_;

        public:
            //コンストラクタ
            RCourseStandard();

            bool capture();
    };
}

#endif
