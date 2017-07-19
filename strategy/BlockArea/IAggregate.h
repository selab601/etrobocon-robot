#ifndef IAGGREGATE_H_
#define IAGGREGATE_H_

namespace strategy{

/**
 *  @brief イテレータを使用するブロックエリアmapのインターフェースクラス
 */
 class IAggregate{
    public:
        virtual IIterator* Iterator(void) = 0;

 };

}
#endif
