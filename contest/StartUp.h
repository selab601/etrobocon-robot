/**
 * @file StartUp.h
 * @brief スタートアップクラス
 * @author motoki nagaoka
 * @date 2015/07/08
 */


#ifndef START_UP_
#define START_UP_

#include "../device/ColorSensor.h"
#include "../device/TouchSensor.h"
#include "../device/Display.h"

namespace contest{
	class StartUp{
		public:
		/**
		* @brief インスタンスを取得する．
		* @details シングルトンパターンに基づいて，共有のインスタンスを取得する．
		* @return スタートアップクラスのインスタンス
		*/
		static StartUp* getInstance();

		/**
		* @brief スタートアップする
		*
		*
		*/
		bool startUp();
		/**
		 * @brief 選択されたコースを取得する
		 * @details Lコース：L, Rコース：R
		 *
		 * @return 'L', または 'R'
		 * @author motoki nagaoka
		 */
		char getSelectedCourse();

	private:

		static StartUp* instance;	// インスタンス

		char selectedCourse = 0;	// 選択されたコース (Lコース； 'L', Rコース：'R')

		// 走行体情報
    device::ColorSensor* brightnessInfo;
		// Gyro* gyro;  // test
    device::TouchSensor* touch;
    device::Display* display;

		//コンストラクタ
		StartUp();

		bool calibrate();		// キャリブレーションする
		bool selectCourse();	// コースを選択する

	};
}
#endif
