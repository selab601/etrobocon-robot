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

    const char* BlockAreaCoordinate::toString() {
        // TODO: fix magic number
        char buf[7];
        sprintf(buf,"(%1d,%1d)", x_, y_);

        char *str_to_ret = (char *)malloc(sizeof(char) * 7);
        for (int i=0; i<7; i++) {
            str_to_ret[i] = buf[i];
        }
        return str_to_ret;
    }
}
