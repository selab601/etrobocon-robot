/*
 * @brief 直進走行クラス
 */

#ifndef STRAIGHT_RUNNING_H_
#define STRAIGHT_RUNNING_H_

#include "device/Motors.h"
#include "ev3api.h"

namespace drive{

	class StraightRunning{

		private:
			device::Motors* motor_;

		public:			
			//コンストラクタ
			StraightRunning();
			
			/**
			 * @brief 直進する．
		 	 * @param speed (-100~100)．	正の値で前進，負の値で後退
			 */
			void run(int speed);
	};
};
#endif