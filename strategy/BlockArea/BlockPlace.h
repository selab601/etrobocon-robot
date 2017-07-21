#ifndef BLOCKPLACE_H_
#define BLOCKPLACE_H_

#include <vector>

namespace strategy{

    enum class BlockAreaColor{
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
        BlockAreaColor color_;//台座の色
        //int x_;//x座標
        //int y_;//y座標
        //bool hasBlock_;//ブロックの有無
        //color hasBlockColor;//台座上のブロックの色
        std::vector<int> nextBlockPlace;//[接続してる置き場]-------２次元にしたい[その角度]


    public:
            //ブロック並べに使用する色の種類



        //コンストラクタ
        BlockPlace(int id,BlockAreaColor color);

        //void setId(int id);///?
        //void setcolor(int color);//?

        /**
         * @brief 繋がっているブロック置き場の登録
         * @param id    ブロック置き場ID
         * @param angle ラインの角度
         */
        void connectPlace(int id);

        /**
         * @brief 繋がっている次のブロック置き場を取得
         * @return ブロック置き場ID
         */
        //vector<int> getNextPlace();

    };

}

#endif
