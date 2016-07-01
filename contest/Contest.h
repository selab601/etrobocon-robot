/**
 * @file Contest.h
 * @brief 競技クラス
 * @author usui kakeru
 */
#ifndef	 CONTEST_
#define CONTEST_

#include "StartUp.h"
#include "Course.h"

namespace contest_pkg {

	class Contest {
		public:

		private:
			//インスタンス
			static Contest* instance;

			//スタートアップ
			StartUp* startUp;

			//コース
			Course* courseL;
			Course* courseR;

		public:
			/**
			 * @brief インスタンス取得
			 * @return インスタンス
			 */
			static Contest* getInstance();

			/**
			 * @brief 競技を行う
			 * 走行体を制御します。
			 * 現在のコースに合わせて戦略を実行し、コースを攻略します。
			 */
			void perform();

		private:
			/**
			 * @brief コンストラクタ
			 * 外部から呼び出せません
			 */
			Contest();
	};
}
#endif
