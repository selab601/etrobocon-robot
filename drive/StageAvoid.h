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
	 * @台座回避走行クラス 
	 **/
	class StageAvoid{
	private:
		CurveRunning* curveRunning_;
		StraightRunning* straightRunning_;
		detection::LineDetection* lineDetection_;
		measurement::BodyAngleMeasurement* bodyAngle_;

	public:
		//コンストラクタ
		StageAvoid();

		/**
		 * @台座回避を実行する
		 **/
		void startAvoidance(DirectionKind KIND);

		/**
		 * @右折しながら台座回避
		 **/
		void right();

		 /**
		 * @左折しながら台座回避
		 **/
		void left();

		 /**
		 * @直進、右から回り込んで台座回避
		 **/
		void straightRight();

		 /**
		 * @直進、左から回り込んで台座回避
		 **/
		void straightLeft();

	};

}

#endif