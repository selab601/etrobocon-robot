#ifndef MAP_H_
#define MAP_H_

#include "BlockPlace.h"
#include "BlockAreaLine.h"
#include "BlockCode.h"
#include <vector>
#include <unordered_map>

class TestIterator;

namespace strategy{

    class Map{
    private:
        strategy::BlockCode* blockCode_; //ブロックの初期位置
        std::unordered_map<int,BlockPlace*> blockPlaces_;//置き場の集合

        std::vector<BlockPlace*> routeBlockPlace_;//ルート(台座の羅列)
        std::vector<int> routeDigree_;//ルート(角度の羅列)
        std::vector<int> routeHasBlock_;//ブロック持ってるかどうかのフラグ0:持ってない 1:持ってる


        std::unordered_map<std::string,BlockPlace*> blockIs_;//ブロックの現在地
        std::unordered_map<std::string,BlockPlace*> blockDestination_;//ブロックの目的地

        BlockPlace* ev3Is_;        //EV3の現在地

        BlockPlace* nextCarryBlockIs_ = NULL; //次に運ぶブロックの場所
        BlockPlace* nextCarryDestination_ = NULL; //次に運ぶブロックの目的地

        //運べるブロック
        bool flag_red;
        bool flag_blue;
        bool flag_green;
        bool flag_yellow;
        bool flag_black;

        /**
         * @brief ５つのブロックを運び終えたか確認
         * @return true:終了
         *         false:まだ終わってない
         */
        bool checkFinish();

        /**
         * @brief makePath 現在のブロック置き場からgoal位置のブロック置き場までの1本のpathを計算して、routeBlockPlace_に継ぎ足してく
         * @param goal  移動先のブロック置き場
         */
        void makePath(BlockPlace* goal);

        /**
         * @brief 次に運ぶブロックを選択して、値をsetする
         *        nextCarryBlockIs 次運ぶブロックの場所
         *        nextCarryDestination 次運ぶブロックの目的地
         * @return 次に運ぶブロックの場所
         */
        void selectCarryBlock();

    public:

        //コンストラクタ
        Map();

        //ルート作る(5角形に何にもない時用)
        void makeRoute1();


        std::vector<BlockPlace*> getrouteBlockPlace();
        std::vector<int> getRouteDigree();
        std::vector<int> getRouteHasBlock_();
    };

}

#endif
