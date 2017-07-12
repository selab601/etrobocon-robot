#ifndef BLOCKCODE_
#define BLOCKCODE_

#include "math.h"
#include "stdio.h"

namespace contest_pkg {
    class BlockCode{
    private:

        static BlockCode* instance_;
        BlockCode();

        int red_;
        int blue_;
        int yellow_;
        int green_;
        int black_;
        
        

    public:
        static BlockCode* getInstance();

        void setCode(int initPositionCode);

        int getIdRed();
        int getIdBlue();
        int getIdYellow();
        int getIdGreen();
        int getIdBlack();
    };

}
#endif