#include "BlockAreaEntry.h"
#include "../drive/Catching.h"


namespace strategy{
    BlockAreaEntry::BlockAreaEntry(){
        linetrace_ = drive::LineTrace::getInstance();
        distanceMeasurement_ = measurement::DistanceMeasurement();
        motor_ = device::Motors::getInstance();
        pivotTurn_ = drive::PivotTurn();
        straightRunning_ = drive::StraightRunning();
        lineDetection_ = detection::LineDetection();
        curveRunning_ = drive::CurveRunning();
        bodyAngleMeasurement_ = measurement::BodyAngleMeasurement();
        colorDetection_ = detection::ColorDetection();

        selfPositionEstimation_ = measurement::SelfPositionEstimation::getInstance();

        Status_ = Status::STANDBY;

    }

    bool BlockAreaEntry::capture(){
        static drive::Catching catching = drive::Catching();
        switch(Status_){
            case Status::STANDBY:
                //侵入場所の真横まで
                distanceMeasurement_.setTargetDistance(580);
                distanceMeasurement_.startMeasurement();
                Status_ = Status::LINETRACE;
                break;

            case Status::LINETRACE:
                if(!distanceMeasurement_.getResult()){
                    linetrace_->run(30,drive::LineTraceEdge::LEFT,0.6);
                }else{
                    motor_->setWheelPWM(0,0);
                    Status_ = Status::TURN;
                }
                break;

            case Status::TURN:
                if(pivotTurn_.turn(80)){
                    straightRunning_.initialize();
                    distanceMeasurement_.setTargetDistance(350);
                    distanceMeasurement_.startMeasurement();
                    Status_ = Status::STRAIGHT_SPEED_UP;
                }
                break;

                // 35cm進むまでPWM50
            case Status::STRAIGHT_SPEED_UP:
                if (!distanceMeasurement_.getResult()){
                    straightRunning_.run(50, 100);
                }
                else{
                    straightRunning_.initialize();
                    Status_ = Status::STRAIGHT_SPEED_DOWN;
                }
                break;

                // 35cm進んだら減速する
            case Status::STRAIGHT_SPEED_DOWN:
                //ライン検知するまで
                if(!lineDetection_.getResult()){
                    straightRunning_.run(20, 100);
                }else{
                    //信地旋回準備
                    bodyAngleMeasurement_.setBaseAngle();
                    Status_ = Status::TURN2;
                }
                break;

            case Status::TURN2:
                //90°信地旋回
                if(bodyAngleMeasurement_.getResult() >= 85){
                    Status_ = Status::INITIALIZE;
                }else{
                    curveRunning_.run(30, -2);
                }
                break;

            case Status::INITIALIZE:
                if (catching.putBlock()){
                    Status_ = Status::DONE;
                }
                break;

            case Status::DONE:
                motor_->setWheelPWM(0,0);
                selfPositionEstimation_->initMap();
                return true;
        }
        return false;
    }
};
