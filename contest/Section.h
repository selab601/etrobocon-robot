/**
 * @file Section.h
 * @brief 区間クラス
 * @author usui kakeru
 */
#ifndef SECTION_H_
#define SECTION_H_

//#include "../underlyingTechnology/Localization.h"
#include "ev3api.h"
#include "../strategy/IStrategy.h"
#include "stdio.h"


namespace contest_pkg{

	class Section{
		private:
			//区間距離
			int sectionDistance;
			
			//戦略完了
			bool isCaptured;
			
			//走行距離確認完了
			bool isChecked;

			// 相対距離を使って区間を切り替えるかどうか
			bool useRelativeDistance;

			// スタートした位置
			int startDistance;

			// スタートしたかどうか
			int isStarted;
			
			//戦略：戦略
			strategy::IStrategy* strategy;

			//自己位置推定
			//Localization* localization;

		public:
			/**
			 * @brief コンストラクタ
			 * @details 区間距離と戦略を割り当てる
			 *
			 * @param sectionDistance 区間距離
			 * @param strategy 戦略
			 * @param useRelativeDistance 相対距離を使って区間を切り替えるとき:true
			 */
			Section(int sectionDistance,strategy::IStrategy* strategy, bool useRelativeDistance = false);
			/**
			 * @brief 区間を攻略する
			 * @details 戦略クラスの攻略するを呼び出す
			 * @return 攻略済み:true,攻略中:false
			 */
			bool capture();

			/**
			 * @brief 走行距離を確認する
			 * @details 区間距離と自己位置の差を計算する
			 * @return 自己位置が区間距離を越えている:true,越えていない:false
			 */
			bool checkDistance();

	};
}
#endif
