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
        int x_;//x座標 mm
        int y_;//y座標 mm

    public:
            //ブロック並べに使用する色の種類
        BlockPlace* leftAngle0_    = NULL;
        BlockPlace* leftAngle30_   = NULL;
        BlockPlace* leftAngle60_   = NULL;
        BlockPlace* leftAngle75_   = NULL;
        BlockPlace* leftAngle105_  = NULL;
        BlockPlace* leftAngle120_  = NULL;
        BlockPlace* leftAngle150_  = NULL;
        BlockPlace* leftAngle180_  = NULL;

        BlockPlace* rightAngle30_  = NULL;
        BlockPlace* rightAngle60_  = NULL;
        BlockPlace* rightAngle75_  = NULL;
        BlockPlace* rightAngle105_ = NULL;
        BlockPlace* rightAngle120_ = NULL;
        BlockPlace* rightAngle150_ = NULL;



        //コンストラクタ
        BlockPlace(int id,BlockAreaColor color,int x,int y);

        //void setId(int id);///?
        //void setcolor(int color);//?


    };

}

#endif
