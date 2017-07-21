#ifndef TESTITERATOR_H_
#define TESTITERATOR_H_

class Map;

namespace strategy{

 class TestIterator : public IIterator{
    private:
        Map map_;//ブロック並べのmap
    public:
        TestIterator(Map map);
        bool test();
 };
}
#endif
