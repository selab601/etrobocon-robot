#include "Map.h"
#include "TestIterator.h"

namespace strategy{

    Map::Map(){}

    IIterator* Map::iterator(){
        return TestItertor();
    }

    void Map::addBlockPlace(BlockPlace blockPlace){
        map_.emplace_back(blockPlace);
    }

    BlockPlace Map::getBlockPlaceAt(int blockPlaceId){
        return map_[blockPlaceId];
    }

}
