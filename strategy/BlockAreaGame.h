/*
 * @brief エリア内でのゲーム課題用戦略
 */

#ifndef _BLOCK_AREA_GAME_H
#define _BLOCK_AREA_GAME_H

#include "IStrategy.h"
#include "BlockArea/Map.h"
#include "BlockArea/IIterator.h"

namespace strategy{
	class BlockAreaGame : public IStrategy{
		private:
            strategy::Map* map_;//ブロック並べのmap
            //strategy::IIterator* itertor_;

		public:
			//コンストラクタ
            //ここでMapの内容を定義する
			BlockAreaGame();

			bool capture();

            //Todo
            //計算するIterator呼ぶとかmoveとかいろいろメソッド作る

	};
}

#endif
