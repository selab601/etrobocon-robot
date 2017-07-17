/*
 * @brief エリア内でのゲーム課題用戦略
 */

#ifndef _BLOCK_AREA_GAME_H
#define _BLOCK_AREA_GAME_H

#include "IStrategy.h"


namespace strategy{
	class BlockAreaGame : public IStrategy{
		private:

		public:
			//コンストラクタ
			BlockAreaGame();

			bool capture();

	};
}

#endif
