#ifndef BLOCKPLACE_H_
#define BLOCKPLACE_H_

#include <vector>

namespace strategy{

    enum color
    {
        RED,
        BLUE,
        YELLOW,
        GREEN,
        BLACK
    };

    class BlockPlace{
    private:
        //ToDo 必要な情報変えてく
        int id_;//置き場のID
        color color_;//台座の色
        bool hasBlock_;//ブロックの有無
        std::vector<int> nextBlockPlace;//[接続してる置き場]-------２次元にしたい[その角度]


    public:
        //コンストラクタ
        BlockPlace(int id);

        //void setId(int id);///?
        //void setcolor(int color);//?

        /**
         * @brief 繋がっているブロック置き場の登録
         * @param id    ブロック置き場ID
         * @param angle ラインの角度
         */
        void connectPlace(int id);

    };

}

#endif
