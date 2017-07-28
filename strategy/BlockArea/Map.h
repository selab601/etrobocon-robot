#ifndef MAP_H_
#define MAP_H_

#include "BlockPlace.h"
#include "BlockCode.h"
#include <string>
#include <vector>
#include <unordered_map>

#include "../../drive/Catching.h"
#include "../../drive/Avoidance.h"

class TestIterator;

namespace strategy{

    class Map{
    private:
        strategy::BlockCode* blockCode_; //ブロックの初期位置
        std::unordered_map<int,BlockPlace*> blockPlaces_;//置き場の集合

        BlockPlace* ev3Is_;        //EV3の現在地
        bool ev3HasBlock_;//ブロック持ってるかどうかのフラグ false:持ってない true:持ってる
        const int ev3DegreeAtEntry_ = 60;//ブロックエリアに入る時の車体角度

        std::unordered_map<std::string,BlockPlace*> blockIs_;//ブロックの現在地
        std::unordered_map<std::string,BlockPlace*> blockDestination_;//ブロックの目的地
        // BlockPlace* nextCarryBlockIs_ = NULL; //次に運ぶブロックの場所
        // BlockPlace* nextCarryDestination_ = NULL; //次に運ぶブロックの目的地
        std::string nextCarryBlockColor_;//次に運ぶブロックの色

        enum class MovePattern{
            CATCH,
            AVOID,
            PUT
        };
        std::vector<BlockPlace*> routeBlockPlace_;//ルート(台座の羅列)
        std::vector<MovePattern> routeMovePattern_;//ルートを通る時の行動パターン 下のMovePatternが入る



        drive::Catching* catching_;
        drive::Avoidance* avoidance_;

        enum class PutProcess{
            PUT,
            AVOID,
            END
        };
        PutProcess putProcess_ = PutProcess::PUT;//上のMovePattern でのPUT動作 の処理内容




        //運べるブロック  使ってない
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
         * @brief ブロックがその置き場に置かれているか確認
         * @param  checkPlace 確認する置き場
         * @return true  : ある
         *         false : ない
         */
        bool checkBlock(BlockPlace* checkPlace);

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

        /**
         * @brief ルートを作る(5角形に何にもない時用)
         */
        void makeRoute1();

        /**
         * @brief pathをもとにdrive::Catching とdrive::Avoidanceを実行する
         * @return true  : 完了
         *         false : 未完了
         */
        bool runPath();

    };

}

#endif
