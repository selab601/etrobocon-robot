#include "RegulateDistanceStage.h"

namespace drive{
    RegulateDistanceStage::RegulateDistanceStage(){
        selfPositionEstimation_ = measurement::SelfPositionEstimation::getInstance();
        distanceMeasurement_ = new measurement::DistanceMeasurement();
        straightRunning_ = new StraightRunning();
        initialized_ = false;
    }
    bool RegulateDistanceStage::run(){
        //現在地を取得
        x_ = abs(selfPositionEstimation_->getLocationX());
        y_ = abs(selfPositionEstimation_->getLocationY());
        //超信地旋回後の処理
        if(x_ < 100 && y_ < 100)return true;
        //
        if(!initialized_){
            if(y_ < 100){
                //直進時の目標位置をセット(X座標)
                regulateDistance_ = 450;
                position_ = x_;
            }
            else{
                //右左折時の目標位置をセット(Y座標)
                regulateDistance_ = 230;
                position_ = y_;
            }
            int diff = abs(regulateDistance_ - position_);
            if (diff > 20){
                diff = 20;
            }
            //目標の位置より前方にいるので後退
            if(position_ >= regulateDistance_){
                //下がれる距離の限界を超えているか
                if((position_ - regulateDistance_) > MAXBACKDISTANCE){
                    moveDistance_ = MAXBACKDISTANCE;
                }
                else{
                    moveDistance_ = position_ - regulateDistance_;
                }
                straightRunning_->run(-diff/2 -3);
            }
            //目標の位置より後方にいるので前進
            else if(position_ < regulateDistance_){
                moveDistance_ = regulateDistance_ - position_;
                straightRunning_->run(diff + 3);
            }
                distanceMeasurement_->setTargetDistance(moveDistance_);
                distanceMeasurement_->startMeasurement();
                initialized_ = true;
        }

        if(distanceMeasurement_->getResult()){
            straightRunning_->run(0);
            initialized_ = false;
            return true;
        }
        return false;
    }
}
