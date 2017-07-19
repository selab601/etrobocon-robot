#ifndef IITERATOR_H_
#define IITERATOR_H_

namespace strategy{

/**
 *  @brief イテレータ用インターフェースクラス
 *  標準にあるitertorクラスを使うなら派生クラスにする必要ある
 */
 class IIterator{
    public:
        //一例
        //virtual bool IsLast(void)=0;//最後の要素かどうか
        //virtual Book Next(void)=0;//イテレータが指し示すオブジェクトを返して、次のオブジェクトを指す
        virtual bool next(void) = 0;//次の要素があるか
 };

}
#endif
