#include "BlockColorCheck.h"

namespace drive {
    BlockColorCheck::BlockColorCheck() {
        executingState_ = new DetectTableState();
        states_.push(new DetectBlockState());
    }

    bool BlockColorCheck::isExecuted(colorset_t* result) {
        // 実行中
        if (executingState_->executed(result) == false) {
            return false;
        }

        // 実行終了
        if (states_.empty()) {
            return true;
        } else {
            executingState_ = states_.front();
            states_.pop();
        }

        return false;
    }
}
