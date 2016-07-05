/**
 * @file Section.cpp
 * @brief 区間クラス
 * @author 薄井　翔
 */
#include "Section.h"

using namespace device;
using namespace strategy;
using namespace measurement;

namespace contest_pkg{


    /* コンストラクタ */
    Section::Section(int sectionDistance, IStrategy* strategy, bool useRelativeDistance){
        this->sectionDistance = sectionDistance;
        this->strategy = strategy;
        this->useRelativeDistance = useRelativeDistance;
        isCaptured = false;
        isChecked = false;
        isStarted = false;
        startDistance = 0;

        selfPositionEstimation = SelfPositionEstimation::getInstance();
        display = Display::getInstance();
    }

    /* 区間攻略 */
    bool Section::capture(){
        char message[30];
        sprintf (message, "rel:%d, start:%d, dst:%d", useRelativeDistance, startDistance, sectionDistance);
        display->updateDisplay(message,0);
        // 区間の始まりの位置を記録する
        if ( ! isStarted ){
            startDistance = selfPositionEstimation->getMigrationLength();
            isStarted = true;
        }
        //戦略を攻略する
        if ( strategy->capture() ){
            isCaptured = true;
        }

        if ( checkDistance() ){
        isChecked = true;
        }

        if ( isCaptured && isChecked == true ){
            return true;
        }

        return false;
    }

    /* 走行距離確認 */
    bool Section::checkDistance(){
        // 絶対距離を使うとき
        if ( !useRelativeDistance ){
            return  selfPositionEstimation->getMigrationLength() > sectionDistance;
        }

        // 相対距離を使うとき
        else {
            return (selfPositionEstimation->getMigrationLength() - startDistance) > sectionDistance;
        }
    }
}
