#ifndef MAP_H_
#define MAP_H_

namespace strategy : public IAggregate{

    class Map{
    private:
        std::vector<BlockPlace> map_;
    public:
        /**
         * @brief 引数にブロック置き場の総数を指定する
         * @param maxSize ブロック置き場の数
         */
        Map(int maxSize);

        /*
         * @brief mapのiteratorを返す
         */
        iterator();
        /**
         * @brief ブロック置き場をmapに追加する
         * @param BlockPlace 追加するブロック置き場
        */
        addBlockPlace(BlockPlace blockPlace);
        /**
         * @brief 指定したIDのブロック置き場の情報を返す
         * @return BlockPlace
         */
        BlockPlace getBlockPlaceAt(int blockPlaceId);

    }


}
