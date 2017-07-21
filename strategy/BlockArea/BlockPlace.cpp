#include "BlockPlace.h"

namespace strategy{
    BlockPlace::BlockPlace(int id){
        id_ = id;
    }

    void BlockPlace::connectPlace(int id){
        nextBlockPlace.emplace_back(id);
    }


}
