#include "TestIterator.h.h"
#include "Map.h"

namespace strategy{


    TestIterator::TestIterator(Map map){
        map_ = map;
    }
    bool TestIterator::test(){
        return true;
    }


}
