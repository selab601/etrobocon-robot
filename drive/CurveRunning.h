/*
 * @brief カーブ走行クラス
 */

#ifndef CURVE_RUNNING_H_
#define CURVE_RUNNING_H_

#include "device/Motors.h"
#include "ev3api.h"

namespace drive{

	class CurveRunning{

		private:
			device::Motors* motor_;

		public:
			//コンストラクタ
			CurveRunning();

			/**
			 * @brief カーブ走行  引数に左右のタイヤのPWM値を直接入れる  
			 　　　　　　　　　　　 今後拡張するかもしれないですが，わかりやすさ，変更しやすさを考え，とりあえずこの形に…
			 * 		
		 	 * @param speed (-100~100)．	正の値で前進，負の値で後退
			 */
			void run(int Rspeed,int Lspeed);
	};
};
#endif