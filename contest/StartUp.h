/**
 * @file StartUp.h
 * @brief スタートアップクラス
 * @author usui kakeru
 */


#ifndef START_UP_
#define START_UP_

#include "../device/ColorSensor.h"
#include "../device/TouchSensor.h"
#include "../device/Display.h"

namespace contest_pkg{
	class StartUp{
		public:
		/**
		* @brief インスタンスを取得する．
		* @details シングルトンパターンに基づいて，共有のインスタンスを取得する．
		* @return スタートアップクラスのインスタンス
		*/
		static StartUp* getInstance();

		/**
		* @brief コース選択とキャリブレーションの終了を確認
		* @return 終了したらtrue
		*
		*/
		bool isFinished();
		/**
		 * @brief 選択されたコースを取得する
		 * @details Lコース：L, Rコース：R
		 *
		 * @return 'L', または 'R'
		 * @author motoki nagaoka
		 */
		char getSelectedCourse();

		/**
		 * @brief スタートを受け付ける
		 * @details タッチセンサが押されるまでまつ
		 * @return タッチセンサが押されたらtrue
		 */
		bool acceptStart();

	private:

		static StartUp* instance_;	// インスタンス

		//コンストラクタ
		StartUp();

		char selectedCourse_ = 0;	// 選択されたコース (Lコース； 'L', Rコース：'R')

		// 走行体情報
		device::ColorSensor* brightnessInfo_;
		device::TouchSensor* touch_;
		device::Display* display_;

		//キャリブレーションする色
		enum calibrationColor
		{
			WHITE,
			BLACK
		};
		bool calibrate();		// キャリブレーションする
		bool selectCourse();	// コースを選択する
		/**
		 * @brief 色のキャリブレーションをする
		 * @return タッチボタンが押された時のカラーセンサの値
		 */
		int8_t colorCalibrate(calibrationColor color);

	};
}
#endif
