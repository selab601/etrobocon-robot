#ifndef _BLOCK_AREA_COORDINATE_
#define _BLOCK_AREA_COORDINATE_

#include <stdio.h>
#include <stdlib.h>

namespace drive{
    class BlockAreaCoordinate {
    private:
        int x_;
        int y_;
    public:
        BlockAreaCoordinate();
        BlockAreaCoordinate(int x,int y);

        int getX();
        int getY();

        const char* toString();

        BlockAreaCoordinate operator-(BlockAreaCoordinate &cor) {
            return BlockAreaCoordinate(x_-cor.getX(), y_-cor.getY());
        }

        bool operator==(BlockAreaCoordinate &cor) {
            return x_ == cor.getX() && y_ == cor.getY();
        }
    };
};


#endif
