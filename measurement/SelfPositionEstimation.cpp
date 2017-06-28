#include "SelfPositionEstimation.h"

namespace measurement
{
    using namespace ev3api;

    SelfPositionEstimation* SelfPositionEstimation::_instance = 0;

    SelfPositionEstimation* SelfPositionEstimation::getInstance(){
        if(_instance == 0){
            _instance = new SelfPositionEstimation();
        }
        return _instance;
    }

    SelfPositionEstimation::SelfPositionEstimation(){
    location_ = Coordinates();
    measurePoint_ = Coordinates();
    clock_ = Clock();
    angle_  = 0;
    deltaTime_ = 0;
    leftBuf_ = 0;
    rightBuf_ = 0;
    velocityEV3_ = 0;
    angularEV3_ = 0;
    angularVL_ = 0;
    angularVR_ = 0;
    migrationLength_ = 0;
    ml_ = 0;
    }

    //更新
    void SelfPositionEstimation::update(int32_t countL,int32_t countR){
        //増分時間の計算，左項のdeltaTime_は前回の呼び出し時間になっている
        deltaTime_ = clock_.now() - deltaTime_;

        angularVL_ = (double)(countL - leftBuf_)*M_PI*2 / (double)(ENCORDER_GEAR*deltaTime_);
        angularVR_ = (double)(countR - rightBuf_)*M_PI*2 / (double)(ENCORDER_GEAR*deltaTime_);

        //EV3本体の速度，回転速度計算
        velocityEV3_ = (WHEEL_RADIUS/2)*angularVL_ + (WHEEL_RADIUS/2)*angularVR_;
        angularEV3_ =  ((double)WHEEL_RADIUS/(double)TREAD)*angularVR_ - ((double)WHEEL_RADIUS/(double)TREAD)*angularVL_;

        //向きの計算
        angle_ += angularEV3_ * (double)deltaTime_;
        //位置の計算
        location_.add_xy(velocityEV3_ * cos(angle_) * deltaTime_,velocityEV3_ * sin(angle_) * deltaTime_);
        //移動距離の計算
        ml_ += (velocityEV3_ > 0 ? velocityEV3_ : (-1) * velocityEV3_) *  (double)deltaTime_;
        migrationLength_ += (int)ml_;
        ml_ = ml_ - (int)ml_;

        //エンコーダ値バッファ更新
        leftBuf_ = countL;
        rightBuf_ = countR;
        //呼び出し現在時間保持
        deltaTime_ = clock_.now();
    }
    //座標取得
    Coordinates* SelfPositionEstimation::getLocation(){
        return &location_;
    }
    //x座標
    long SelfPositionEstimation::getLocationX(){
        return location_.get_x();
    }
    //y座標
    long SelfPositionEstimation::getLocationY(){
        return location_.get_y();
    }
    //計測開始
    void SelfPositionEstimation::startMeasure(){
        measurePoint_.set_x(location_.get_x());
        measurePoint_.set_y(location_.get_y());
    }
    //計測地点を原点としたx座標を返す
    long SelfPositionEstimation::getMeasureX(){
        return location_.get_x() - measurePoint_.get_x();
    }
    //計測地点を原点としたy座標を返す
    long SelfPositionEstimation::getMeasureY(){
        return location_.get_y() - measurePoint_.get_y();
    }
    //location_からの距離を返す
    int SelfPositionEstimation::getDistance(){
        return (int)(sqrt(location_.get_x()*location_.get_x() + location_.get_y()*location_.get_y()));
    }
    //移動距離を返す
    long SelfPositionEstimation::getMigrationLength(){
        return migrationLength_;
    }
    //走行体の向いている方向を角度で返す
    int SelfPositionEstimation::getAngle(){
        return (int)(angle_*180/M_PI);
    }
    //自己位置初期化
    void SelfPositionEstimation::initMap(){
        location_.init();
        measurePoint_.init();
        angle_ = 0;
        velocityEV3_ = 0;
        angularEV3_ = 0;
        angularVL_ = 0;
        angularVR_ = 0;
        migrationLength_ = 0;
        ml_ = 0;
    }
}
