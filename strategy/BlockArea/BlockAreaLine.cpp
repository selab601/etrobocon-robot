#include "BlockAreaLine.h"

namespace strategy{

    BlockAreaLine::BlockAreaLine(int id,int connectA,int connectB,int length,int angle){
        id_       = id;
        connectA_ = connectA;
        connectB_ = connectB;
        length_   = length;
        angle_    = angle;
    }

    int BlockAreaLine::connectTo(int blockPlaceId){
        return blockPlaceId == connectA_ ? connectB_ : connectA_;
    }
    int BlockAreaLine::getLenght(){
        return length_;
    }

    int BlockAreaLine::getAngle(){
        return angle_;
    }


}
