#include "BlockAreaLine.h"

namespace strategy{

    BlockAreaLine::BlockAreaLine(int id,int connectRight,int connectLeft,int length){
        id_       = id;
        connectRight_ = connectRight;
        connectLeft_ = connectLeft;
        length_   = length;
    }

    int BlockAreaLine::connectTo(int blockPlaceId){
        return blockPlaceId == connectRight_ ? connectLeft_ : connectRight_;
    }
    int BlockAreaLine::getLenght(){
        return length_;
    }


}
