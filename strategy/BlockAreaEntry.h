/*
 * @brief ブロック並べエリアへ侵入
 */

#ifndef _BLOCK_AREA_ENTRY_H
#define _BLOCK_AREA_ENTRY_H

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

namespace strategy{
	class BlockAreaEntry : public IStrategy{
		private:
			enum class Status
			{
				STANDBY,
				LINETRACE,
				TURN,
                STRAIGHT_SPEED_UP,
                STRAIGHT_SPEED_DOWN,
				TURN2,
				LINETRACE2,
				BACK,
				DONE
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

		public:
			//コンストラクタ
			BlockAreaEntry();

			bool capture();
	};
}

#endif
