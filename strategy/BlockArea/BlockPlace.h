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
        int x_;//x座標
        int y_;//y座標
        //bool hasBlock_;//ブロックの有無
        //color hasBlockColor;//台座上のブロックの色


    public:
            //ブロック並べに使用する色の種類



        //コンストラクタ
        BlockPlace(int id,BlockAreaColor color,int x,int y);

        //void setId(int id);///?
        //void setcolor(int color);//?


    };

}

#endif
