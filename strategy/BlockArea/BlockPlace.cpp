#include "BlockPlace.h"

namespace strategy{
    BlockPlace::BlockPlace(int id,BlockAreaColor color){
        id_ = id;
        color_ = color;
    }

    void BlockPlace::connectPlace(int id){
        nextBlockPlace.emplace_back(id);
    }

    //vector<int> BlockPlace::getNextPlace(){}

}
