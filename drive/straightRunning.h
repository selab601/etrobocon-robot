#ifndef STRAIGHT_RUNNING_H_
#define STRAIGHT_RUNNING_H_

#include "device/Motors.h"
#include "ev3api.h"

namespace drive{
	using namespace device;

	class straightRunning{

		private:
			Motors* motor_;

			//インスタンス
			static straightRunning* instance;
			
		public:
			//インスタンス取得
			static straightRunning* getInstance();

			/**
			 * @brief 直進する．
		 	 * @param speed (-100~100)．	正の値で前進，負の値で後退
			 */
			void runStraight(int speed);

		private:
			//コンストラクタ
			straightRunning();
	};
};
#endif