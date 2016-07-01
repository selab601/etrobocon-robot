#ifndef CURVE_RUNNING_H_
#define CURVE_RUNNING_H_

#include "device/Motors.h"
#include "ev3api.h"

namespace drive{
	using namespace device;

	class curveRunning{

		private:
			Motors* motor_;

			//インスタンス
			static curveRunning* instance;
			
		public:
			//インスタンス取得
			static curveRunning* getInstance();

			/**
			 * @brief カーブ走行  引数に左右のタイヤのPWM値を直接入れる  
			 　　　　　　　　　　　 今後拡張するかもしれないですが，わかりやすさ，変更しやすさを考え，とりあえずこの形に…
			 * 		
		 	 * @param speed (-100~100)．	正の値で前進，負の値で後退
			 */
			void runCurve(int Rspeed,int Lspeed);

		private:
			//コンストラクタ
			curveRunning();
	};
};
#endif