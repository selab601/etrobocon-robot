#ifndef IAGGREGATE_H_
#define IAGGREGATE_H_

#include "IIterator.h"

namespace strategy{

/**
 *  @brief イテレータを使用するブロックエリアmapのインターフェースクラス
 */
 class IAggregate{
    public:
        virtual IIterator* iterator(void) = 0;
 };

}
#endif
