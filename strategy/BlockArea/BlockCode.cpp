#include "BlockCode.h"

namespace strategy {

    BlockCode* BlockCode::instance_ = 0;
    BlockCode::BlockCode(){
        block1_ = 0;
        block2_ = 0;
        block3_ = 0;
        block4_ = 0;
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
        powerBlockCode_ =((( initPositionCode - 65536*(block1_) ) - 4096*(block2_) ) - 256*(block3_) ) - 16*(block4_);

        switch(powerBlockCode_){
            case 1:
                isPowerSpot_[0]  = true;
                isPowerSpot_[1]  = true;
                isPowerSpot_[4]  = true;
                isPowerSpot_[5]  = true;
                isPowerSpot_[6]  = true;
                isPowerSpot_[9]  = true;
                isPowerSpot_[10] = true;
                break;
            case 2:
                isPowerSpot_[1]  = true;
                isPowerSpot_[2]  = true;
                isPowerSpot_[5]  = true;
                isPowerSpot_[6]  = true;
                isPowerSpot_[7]  = true;
                isPowerSpot_[10] = true;
                isPowerSpot_[11] = true;
                break;
            case 3:
                isPowerSpot_[4]  = true;
                isPowerSpot_[5]  = true;
                isPowerSpot_[8]  = true;
                isPowerSpot_[9]  = true;
                isPowerSpot_[10] = true;
                isPowerSpot_[13] = true;
                isPowerSpot_[14] = true;
                break;
            case 4:
                isPowerSpot_[5]  = true;
                isPowerSpot_[6]  = true;
                isPowerSpot_[9]  = true;
                isPowerSpot_[10] = true;
                isPowerSpot_[11] = true;
                isPowerSpot_[14] = true;
                isPowerSpot_[15] = true;
                break;
            case 5:
                isPowerSpot_[1]  = true;
                isPowerSpot_[2]  = true;
                isPowerSpot_[4]  = true;
                isPowerSpot_[5]  = true;
                isPowerSpot_[6]  = true;
                isPowerSpot_[8]  = true;
                isPowerSpot_[9]  = true;
                break;
            case 6:
                isPowerSpot_[2]  = true;
                isPowerSpot_[3]  = true;
                isPowerSpot_[5]  = true;
                isPowerSpot_[6]  = true;
                isPowerSpot_[7]  = true;
                isPowerSpot_[9]  = true;
                isPowerSpot_[10] = true;
                break;
            case 7:
                isPowerSpot_[5]  = true;
                isPowerSpot_[6]  = true;
                isPowerSpot_[8]  = true;
                isPowerSpot_[9]  = true;
                isPowerSpot_[10] = true;
                isPowerSpot_[12] = true;
                isPowerSpot_[13] = true;
                break;
            case 8:
                isPowerSpot_[6]  = true;
                isPowerSpot_[7]  = true;
                isPowerSpot_[9]  = true;
                isPowerSpot_[10] = true;
                isPowerSpot_[11] = true;
                isPowerSpot_[13] = true;
                isPowerSpot_[14] = true;
                break;
            default:
                break;
        }
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
        return powerBlockCode_;
    }
    bool BlockCode::isPowerSpot(int placeNum){
        return isPowerSpot_[placeNum];
    }

}
