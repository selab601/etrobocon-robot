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

    public:
            //ブロック並べに使用する色の種類
        BlockPlace* leftAngle0_;
        BlockPlace* leftAngle30_;
        BlockPlace* leftAngle60_;
        BlockPlace* leftAngle75_;
        BlockPlace* leftAngle105_;
        BlockPlace* leftAngle120_;
        BlockPlace* leftAngle150_;
        BlockPlace* leftAngle180_;

        BlockPlace* rightAngle30_;
        BlockPlace* rightAngle60_;
        BlockPlace* rightAngle75_;
        BlockPlace* rightAngle105_;
        BlockPlace* rightAngle120_;
        BlockPlace* rightAngle150_;



        //コンストラクタ
        BlockPlace(int id,BlockAreaColor color,int x,int y);

        //void setId(int id);///?
        //void setcolor(int color);//?


    };

}

#endif
