#ifndef _LCOURSE_STANDARD_H
#define _LCOURSE_STANDARD_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include "device/Motors.h"

namespace strategy{
	class LCourseStandard : public IStrategy{
		private:
			enum class Status
			{
				STANDBY,
				LINETRACE1,
				CHANGEEDGE_TO_LEFT,
				LINETRACE2,
				CHANGEEDGE_TO_RIGHT,
				LINETRACE3,
				DONE
				};
			Status Status_;

			drive::LineTrace* linetrace_;
			measurement::DistanceMeasurement distanceMeasurement_;
			device::Motors* motor_;

		public:
			//コンストラクタ
			LCourseStandard();

			bool capture();
	};
}

#endif