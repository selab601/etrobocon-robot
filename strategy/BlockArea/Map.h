#ifndef MAP_H_
#define MAP_H_

#include "BlockPlace.h"
#include "BlockCode.h"
#include <string>
#include <vector>
#include <unordered_map>

#include "../../drive/Catching.h"
#include "../../drive/Avoidance.h"

//#include "../../communication/BtManager.h"

namespace strategy{

    class Map{
    private:
        strategy::BlockCode* blockCode_; //ブロックの初期位置
        std::unordered_map<int,BlockPlace*> blockPlaces_;//置き場の集合

        BlockPlace* ev3Is_;        //EV3の現在地
        bool ev3HasBlock_;//ブロック持ってるかどうかのフラグ false:持ってない true:持ってる
        const int ev3DegreeAtEntry_ = 60;//ブロックエリアに入る時の車体角度

        std::unordered_map<std::string,BlockPlace*> blockIs_;//ブロックの現在地         【string:ブロックの色 BlockPlace:置き場】
        std::unordered_map<std::string,BlockPlace*> blockDestination_;//ブロックの目的地【string:ブロックの色 BlockPlace:置き場】
        std::unordered_map<std::string,BlockPlace*> blockDisplace_;//ブロックの仮置き場 【string:置き場の色   BlockPlace:置き場】
        std::string nextCarryBlockColor_;//次に運ぶブロックの色

        //ブロック並べエリアで行う動作の種類
        enum class MovePattern{
            CATCH,              //ブロックを掴んで移動 ブロックがない台座を移動 (兼用)
            AVOID,              //ブロック置き場を避けて移動する
            PUT,                //ブロックを置く
            END                 //終了
        };
        std::vector<BlockPlace*> routeBlockPlace_;//ルート(台座の羅列)
        std::vector<MovePattern> routeMovePattern_;//ルートを通る時の行動パターン 下のMovePatternが入る

        bool calculated_;//4mごとに計算しないようにFlag管理

        drive::Catching* catching_;
        drive::Avoidance* avoidance_;

        //ブロックを置く動作(MovePattern::PUT)の実処理手順
        enum class PutProcess{
            PUT,                //ブロックを置く
            AVOID,              //置いた場所を避けて移動
            END
        };
        PutProcess putProcess_ = PutProcess::PUT;//上のMovePattern でのPUT動作 の処理内容

        //デバック用
        char message[100];


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
         * @brief ５角形を作るブロック置き場が埋まっているか確認
         * @return true :全て埋まっている
         *         false:空いている場所がある
         */
        bool checkPentagon();

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
         * @brief ５角形からブロックを移動してデッドロックを解消するpathを足す
         */
        void makeDisplaceBlockPath();

        /**
         * @brief 　エラー回避用メソッド
         *         ブロックエリア進入時には前の置き場情報がないのでAvoidできない
         *         よって、10番ブロック置き場でAvoidを実行しないように必ずどこかにブロックを移動させるpathを作成する
         */
        void makeDodgeAvoidancePath();

        /**
         * @brief 次に運ぶブロックを選択して、値をsetする
         *        nextCarryBlockIs 次運ぶブロックの場所
         *        nextCarryDestination 次運ぶブロックの目的地
         */
        void selectCarryBlock();

        /**
         * @brief 5角形からずらすブロックを選択する
         */
        void selectDisplaceBlock();

        /**
         * @brief 赤ブロックと黒ブロックの目的地を近いとことに修正する
         */
        void selectBlackDestination();

    public:

        //コンストラクタ
        Map();

        /**
         * @brief ルートを作る(5角形に何にもない時用)
         */
        void makeRoute();

        /**
         * @brief 作ったルートをもとにdrive::Catching とdrive::Avoidanceを実行する
         * @return true  : 完了
         *         false : 未完了
         */
        bool runPath();

    };

}

#endif
