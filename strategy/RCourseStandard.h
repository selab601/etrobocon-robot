#ifndef _RCOURSE_STANDARD_H
#define _RCOURSE_STANDARD_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include "device/Motors.h"
#include "measurement/BodyAngleMeasurement.h"
#include "drive/CurveRunning.h"

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

		public:
			//コンストラクタ
			RCourseStandard();

			bool capture();
	};
}

#endif