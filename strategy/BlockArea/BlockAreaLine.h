#ifndef BLOCKAREALINE_H_
#define BLOCKAREALINE_H_

namespace strategy{

    class BlockAreaLine{
    private:
        int id_;
        int connectRight_;
        int connectLeft_;
        int length_;
    public:
        BlockAreaLine(int id,int connectRight,int connectLeft,int length);
        int connectTo(int blockPlaceId);
        int getLenght();

    };

}

#endif
