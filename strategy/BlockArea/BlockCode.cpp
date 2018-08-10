#include "BlockCode.h"

namespace strategy {

    BlockCode* BlockCode::instance_ = 0;
    BlockCode::BlockCode(){
        block1_    = 0;
        block2_   = 0;
        block3_ = 0;
        block4_  = 0;
    }

    BlockCode* BlockCode::getInstance(){
        if(instance_ == NULL){
            instance_ = new BlockCode();
        }
        return instance_;
    }

    void BlockCode::setCode(int initPositionCode){
        //競技規約での変換方法
        block1_ =    initPositionCode / 65536;
        block2_ =(   initPositionCode - 65536*(block1_) ) / 4096;
        block3_ =((  initPositionCode - 65536*(block1_) ) - 4096*(block2_) ) / 256;
        block4_ =((( initPositionCode - 65536*(block1_) ) - 4096*(block2_) ) - 256*(block3_) ) / 16;
        power_block_code_ =((( initPositionCode - 65536*(block1_) ) - 4096*(block2_) ) - 256*(block3_) ) - 16*(block4_);
    }

    int BlockCode::getIdBlock1(){
        return block1_;
    }
    int BlockCode::getIdBlock2(){
        return block2_;
    }
    int BlockCode::getIdBlock3(){
        return block3_;
    }
    int BlockCode::getIdBlock4(){
        return block4_;
    }
    int BlockCode::getIdPowerBlockCode(){
        return power_block_code_;
    }

}
