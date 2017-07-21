#ifndef MAP_H_
#define MAP_H_

#include "BlockPlace.h"
#include "BlockAreaLine.h"
#include <vector>

class TestIterator;

namespace strategy{

    class Map{
    private:
        std::vector<BlockPlace> places_;//ブロック置き場の集合
        std::vector<BlockAreaLine> lines_;//ラインの集合
    public:
        //コンストラクタ
        Map();

        /**
         * @brief ブロック置き場をmapに追加する
         * @param BlockPlace 追加するブロック置き場
        */
        void addBlockPlace(BlockPlace blockPlace);

        /**
         * @brief ブロック置き場をつなげるラインの情報を登録
         * @param line 追加するライン
         */
        void addLine(BlockAreaLine line);

        /**
         * @brief 指定したIDのブロック置き場の情報を返す
         * @return BlockPlace
         */
        BlockPlace getBlockPlaceAt(int blockPlaceId);

    };


}

#endif
