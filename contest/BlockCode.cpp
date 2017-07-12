#include "BlockCode.h"

namespace contest_pkg {

    BlockCode* BlockCode::instance_ = 0;
    BlockCode::BlockCode(){
        red_    = 0;
        blue_   = 0;
        yellow_ = 0;
        green_  = 0;
        black_  = 0;
    }

    BlockCode* BlockCode::getInstance(){
        if(instance_ == NULL){
            instance_ = new BlockCode();
        }
        return instance_;
    }

    void BlockCode::setCode(int initPositionCode){
        black_  = initPositionCode / 1331 + 1;
        red_    = (initPositionCode - 1331*(black_-1) ) / 121 + 1;
        yellow_ = (initPositionCode - 1331*(black_-1) - 121*(red_-1)) / 11 + 1;
        blue_   = initPositionCode - 1331*(black_-1) - 121*(red_-1) - 11*(yellow_-1) + 1;
    }

    int BlockCode::getIdRed(){
        return red_;
    }
    int BlockCode::getIdBlue(){
        return blue_;
    }
    int BlockCode::getIdGreen(){
        return green_;
    }
    int BlockCode::getIdYellow(){
        return yellow_;
    }
    int BlockCode::getIdBlack(){
        return black_;
    }

}