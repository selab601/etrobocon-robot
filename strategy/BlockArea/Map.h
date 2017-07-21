#ifndef MAP_H_
#define MAP_H_

#include "IAggregate.h"
#include "BlockPlace.h"
#include <vector>

class TestIterator;

namespace strategy{

    class Map : public IAggregate{
    private:
        std::vector<BlockPlace> map_;//ブロック置き場の集合
    public:
        //コンストラクタ
        Map();

        /*
         * @brief mapのiteratorを返す
         * とりあえずtest用のiteratorを返す
         */
        IIterator* iterator();

        /**
         * @brief ブロック置き場をmapに追加する
         * @param BlockPlace 追加するブロック置き場
        */
        void addBlockPlace(BlockPlace blockPlace);
        /**
         * @brief 指定したIDのブロック置き場の情報を返す
         * @return BlockPlace
         */
        BlockPlace getBlockPlaceAt(int blockPlaceId);

    };


}

#endif
