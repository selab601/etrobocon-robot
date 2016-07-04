#ifndef STRAIGHT_RUNNING_H_
#define STRAIGHT_RUNNING_H_

#include "device/Motors.h"
#include "ev3api.h"

namespace drive{
	//using namespace device;

	class StraightRunning{

		private:
			device::Motors* motor_;

			//インスタンス
			static StraightRunning* instance;
			
		public:
			//インスタンス取得
			static StraightRunning* getInstance();

			/**
			 * @brief 直進する．
		 	 * @param speed (-100~100)．	正の値で前進，負の値で後退
			 */
			void run(int speed);

		private:
			//コンストラクタ
			StraightRunning();
	};
};
#endif