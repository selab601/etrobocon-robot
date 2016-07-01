/**
 * @file IStrategy.h
 * @brief 戦略クラス
 * @author usui kakeru
 */

#ifndef ISTRATEGY_H_
#define ISTRATEGY_H_

namespace strategy{

    class IStrategy{
        public:
        /**
         * @brief 戦略を攻略する
         * @return true:攻略完了 false:未攻略
         */
        virtual bool WalkThrough() = 0;
    };
}
#endif
