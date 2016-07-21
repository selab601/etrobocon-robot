#ifndef _STAGE_AVOID_
#define _STAGE_AVOID_

#include "detection/LineDetection.h"
#include "measurement/BodyAngleMeasurement.h"
#include "CurveRunning.h"
#include "StraightRunning.h"

namespace drive{
	/**
	 * @台座の避け方の方向の種類
	 **/
	enum class DirectionKind{
		RIGHT,
		LEFT,
		STRAIGHT_RIGHT,
		STRAIGHT_LEFT
	};

	/**
	 * @現在の走行状態
	 **/
	enum class Move{
		START,
		TURN_RIGHT_1,
		TURN_RIGHT_2,
		TURN_LEFT_1,
		TURN_LEFT_2,
		STRAIGHT_1,
		STRAIGHT_2,
	};
	/**
	 * @台座回避走行クラス 
	 **/
	class StageAvoid{
	private:
		CurveRunning* curveRunning_;
		StraightRunning* straightRunning_;
		detection::LineDetection* lineDetection_;
		detection::LineDetection* lineDetection2_;
		measurement::BodyAngleMeasurement* bodyAngle_;
		Move move_;

	public:
		//コンストラクタ
		StageAvoid();

		/**
		 * @brief 台座回避を実行する
		 **/
		bool startAvoidance(DirectionKind KIND);

		/**
		 * @brief 右折しながら台座回避
		 **/
		bool right();

		 /**
		 * @brief 左折しながら台座回避
		 **/
		bool left();

		 /**
		 * @brief 直進、右から回り込んで台座回避
		 **/
		bool straightRight();

		 /**
		 * @brief 直進、左から回り込んで台座回避
		 **/
		bool straightLeft();

	};

}

#endif