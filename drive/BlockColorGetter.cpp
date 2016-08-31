#include "BlockColorGetter.h"

namespace drive {
    BlockColorGetter::BlockColorGetter() {
        // キューに状態を順番に詰める．
        // 今のところは以下の順序
        //   1. table(台座)検知&色取得 (DetectTableState)
        //   2. block検知&色取得 (DetectBlockState)
        // ただし，最初の手順は直接 executingState_ (現在実行中の状態) に格納する
        executingState_ = new DetectTableState();
        states_.push(new DetectBlockState());
        states_.push(new BackToLineState());
    }

    bool BlockColorGetter::isExecuted(colorset_t* result) {
        // 現在の状態(executingState_)実行中
        if (executingState_->isExecuted(result) == false) {
            return false;
        }

        // 現在の状態実行終了
        if (states_.empty()) {
            return true;
        } else {
            executingState_ = states_.front();
            states_.pop();
        }

        return false;
    }
}
