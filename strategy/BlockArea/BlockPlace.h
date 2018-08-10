#ifndef BLOCKPLACE_H_
#define BLOCKPLACE_H_

#include <math.h>
#include <cstdlib>
#include <vector>
#include <unordered_map>

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

    public:
        int x_;//左下(blockplace12)を原点としたx座標
        int y_;//左下(blockplace12)を原点としたy座標
        //繋がってる台座があればhashで管理
        //(next[30] = daiza4 30度の方向に方向に４番の台座が繋がってる)
        std::unordered_map<int,BlockPlace*> next;

        //コンストラクタ
        BlockPlace(int id,BlockAreaColor color,int x,int y);

        //void setId(int id);///?
        //void setcolor(int color);//?

        /**
         * @brief 今いる置き場から次の置き場までの距離を計算する
         * @param  blockPlace 次に進む台座
         * @return            ラインの長さ(距離の長さ)
         */
        int getDistance(BlockPlace* blockPlace);

        /**
         * @brief 目的地までの角度を計算する
         * @param  blockPlace 目的地の置き場
         * @return            現在地から目的地の置き場までの角度
         */
        int getDegree(BlockPlace* blockPlace);

        /**
         * @brief 次に進む置き場を返す
         * @param  angle 目的地への角度
         * @return       目的地に近づける台座
         */
        BlockPlace* getNextPlace(int angle);

        int getId();
    };

}

#endif
