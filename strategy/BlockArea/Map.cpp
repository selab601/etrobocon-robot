#include "Map.h"

namespace strategy{

    Map::Map(){}

    void Map::addBlockPlace(BlockPlace blockPlace){
        map_.emplace_back(blockPlace);
    }

    BlockPlace Map::getBlockPlaceAt(int blockPlaceId){
        return map_[blockPlaceId];
    }

}
