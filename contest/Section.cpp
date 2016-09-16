/**
 * @file Section.cpp
 * @brief 区間クラス
 * @author 薄井翔
 */
#include "Section.h"

using namespace device;
using namespace strategy;
using namespace measurement;

namespace contest_pkg{


    /* コンストラクタ */
    Section::Section(IStrategy* strategy, int sectionDistance, bool useRelativeDistance){
        sectionDistance_ = sectionDistance;
        strategy_ = strategy;
        useRelativeDistance_ = useRelativeDistance;
        isCaptured_ = false;
        isChecked_ = false;
        isStarted_ = false;
        startDistance_ = 0;

        selfPositionEstimation_ = SelfPositionEstimation::getInstance();
        display_ = Display::getInstance();
    }

    /* 区間攻略 */
    bool Section::capture(){
        char message[30];
        sprintf (message, "rel:%d, start:%d, dst:%d", useRelativeDistance_, startDistance_, sectionDistance_);
        display_->updateDisplay(message,0);
        // 区間の始まりの位置を記録する
        if ( ! isStarted_ ){
            startDistance_ = selfPositionEstimation_->getMigrationLength();
            isStarted_ = true;
        }
        //戦略を攻略する
        if ( strategy_->capture() ){
            isCaptured_ = true;
        }

        if ( checkDistance() ){
        isChecked_ = true;
        }

        if ( isCaptured_ && isChecked_ == true ){
            return true;
        }

        return false;
    }

    /* 走行距離確認 */
    bool Section::checkDistance(){
        // 相対距離を使うとき
        if ( useRelativeDistance_ ){
            return (selfPositionEstimation_->getMigrationLength() - startDistance_) > sectionDistance_;
        }
        // 絶対距離を使うとき
        else {
            return  selfPositionEstimation_->getMigrationLength() > sectionDistance_;
        }
    }
}
