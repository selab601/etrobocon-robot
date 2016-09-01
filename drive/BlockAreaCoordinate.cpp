#include "BlockAreaCoordinate.h"

namespace drive{
    BlockAreaCoordinate::BlockAreaCoordinate() {
        x_ = 0;
        y_ = 0;
    }
    BlockAreaCoordinate::BlockAreaCoordinate(int x,int y){
        x_ = x;
        y_ = y;
    }

    int BlockAreaCoordinate::getX(){
        return x_;
    }

    int BlockAreaCoordinate::getY(){
        return y_;
    }
}
