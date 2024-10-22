#ifndef BLOCK_COLOR_CHECK_H_
#define BLOCK_COLOR_CHECK_H_

#include "BlockColorGetterStates/IBlockColorGetterState.h"
#include "BlockColorGetterStates/DetectTableState.h"
#include "BlockColorGetterStates/DetectBlockState.h"
#include "BlockColorGetterStates/BackToLineState.h"
#include "ev3api.h"
#include <queue>

namespace drive {
    class BlockColorGetter {
    private:
        IBlockColorGetterState* executingState_;
        std::queue<IBlockColorGetterState*> states_;

    public:
        BlockColorGetter();

        /**
         * @brief
         *   台座とブロックの色を取得する．
         *   前提条件:
         *     - 呼び出す直前にライントレースしていること
         *     - 読み込む対象の台座にブロックがのっていること
         *   事後条件:
         *     - 走行体の位置は呼び出し位置に戻る
         *     - アームの位置がライントレース可能な位置に戻る
         * @param result 結果(台座とブロックの色)を格納する変数
         * @return 取得が終了したら true, 終了していなければ false
         */
        bool isExecuted(colorset_t* result);
    };
}

#endif
