/**
 * @file Contest.h
 * @author maruyama
 */
#ifndef	 CONTEST_
#define CONTEST_

#include "StartUp.h"
#include "Starter.h"

namespace contest {

	/**
	 * @brief 走行体クラス
	 * control関数を呼び出しと、走行準備(スタートアップ)、コース攻略を行います。
	 * シングルトンによって実装されているので、インスタンスを取得する場合はgetInstanceを呼び出してください。
	 */
	class Contest {
		public:
			/*
			enum TR_STATE
			{
				STARTUP,
				RUN
			};
			*/

		private:
			/* インスタンス */
			static Contest* instance;

			/* スタートアップ */
			StartUp* su;

			/* スタータ */
			Starter* starter;


			/* 状態 */
			// TR_STATE state;

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
