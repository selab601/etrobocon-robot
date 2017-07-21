#ifndef BLOCKAREALINE_H_
#define BLOCKAREALINE_H_

namespace strategy{

    class BlockAreaLine{
    private:
        int id_;
        int connectA_;
        int connectB_;
        int length_;
        int angle_;
    public:
        BlockAreaLine(int id,int connectA,int connectB,int length,int angle);
        int connectTo(int blockPlaceId);
        int getLenght();
        int getAngle();

    };

}

#endif
