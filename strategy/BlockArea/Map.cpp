#include "Map.h"

namespace strategy{

    Map::Map(){}

    void Map::addBlockPlace(BlockPlace blockPlace){
       places_.emplace_back(blockPlace);
    }

    void Map::addLine(BlockAreaLine line){
        lines_.emplace_back(line);
    }

    BlockPlace Map::getBlockPlaceAt(int blockPlaceId){
        return places_[blockPlaceId];
    }

}
