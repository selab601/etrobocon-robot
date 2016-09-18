/*
 * @brief ブロック並べエリアから退場
 */

#ifndef _BLOCK_AREA_EXIT_H
#define _BLOCK_AREA_EXIT_H

#include "IStrategy.h"
#include "drive/LineTrace.h"
#include "measurement/DistanceMeasurement.h"
#include "device/Motors.h"
#include "drive/PivotTurn.h"
#include "drive/StraightRunning.h"
#include "detection/LineDetection.h"
#include "drive/CurveRunning.h"
#include "measurement/BodyAngleMeasurement.h"
#include "detection/ColorDetection.h"

#include "drive/Destination.h"
#include <string.h>

namespace strategy{
	class BlockAreaExit : public IStrategy{
		private:
			enum class Status
			{
				CONFIRM_EV3_POSITION,
				FROM_UP,
				FROM_LEFT1,
				FROM_LEFT2,
				FROM_LEFT3,
				STRAIGHT_SPEED_UP,
                STRAIGHT_SPEED_DOWN,
				TURN,
				LINETRACE
				};
			Status Status_;

			drive::LineTrace* linetrace_;
			measurement::DistanceMeasurement distanceMeasurement_;
			device::Motors* motor_;
			drive::PivotTurn pivotTurn_;
			drive::StraightRunning straightRunning_;
			detection::LineDetection lineDetection_;
			drive::CurveRunning curveRunning_;
			measurement::BodyAngleMeasurement bodyAngleMeasurement_;
			detection::ColorDetection colorDetection_;

			drive::Destination* destination_;
		public:
			//コンストラクタ
			BlockAreaExit();

			bool capture();
	};
}

#endif
