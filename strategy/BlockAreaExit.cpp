#include "BlockAreaExit.h"


namespace strategy{
  BlockAreaExit::BlockAreaExit(){
    linetrace_ = drive::LineTrace::getInstance();
    distanceMeasurement_ = measurement::DistanceMeasurement();
    motor_ = device::Motors::getInstance();
    pivotTurn_ = drive::PivotTurn();
    straightRunning_ = drive::StraightRunning();
    lineDetection_ = detection::LineDetection();
    curveRunning_ = drive::CurveRunning();
    bodyAngleMeasurement_ = measurement::BodyAngleMeasurement();
    colorDetection_ = detection::ColorDetection();

    destination_ = drive::Destination::getInstance();

    Status_ = Status::TO_DESTINATION;
  }

  bool BlockAreaExit::capture(){
    switch(Status_){
      case Status::TO_DESTINATION:
        //(4,4)へ移動
        if(destination_->runTo(4,4)){
          Status_ = Status::CONFIRM_EV3_POSITION;
        }
        break;

      case Status::CONFIRM_EV3_POSITION:
        //取りうる車体位置は二通り UP or LEFT
        if(!strcmp(destination_->getEV3Position(),"UP")){
          Status_ = Status::FROM_UP;
        }else if(!strcmp(destination_->getEV3Position(),"LEFT")){
          Status_ = Status::FROM_LEFT1;
        }
        break;

      case Status::FROM_UP:
        //超信地旋回
        if(pivotTurn_.turn(90)){
          straightRunning_.initialize();
          distanceMeasurement_.setTargetDistance(200); //ライン間は30cm
          distanceMeasurement_.startMeasurement();
          Status_ = Status::STRAIGHT_SPEED_UP;
        }
        break;

      case Status::FROM_LEFT1:
        //下から直接ラインへ
        if(pivotTurn_.turn(-30)){
          distanceMeasurement_.setTargetDistance(200); //ライン間は30cm
          distanceMeasurement_.startMeasurement();
          Status_ = Status::FROM_LEFT2;
        }
        //or上に行って，UP_TO_EXITLINEの戦略へ
        break;

      case Status::FROM_LEFT2:
        if(!lineDetection_.getResult()){
          straightRunning_.run(20);
        }else{
          motor_->setWheelPWM(0,0);
          Status_ = Status::FROM_LEFT3;
        }
        break;

      case Status::FROM_LEFT3:
        if(pivotTurn_.turn(30)){
          distanceMeasurement_.setTargetDistance(280); //ラインまで38cm
          distanceMeasurement_.startMeasurement();
          Status_ = Status::STRAIGHT_SPEED_UP;
        }
        break;

      case Status::STRAIGHT_SPEED_UP:
        //直進走行→ライン検知→信地旋回→角度検知（orライン検知）
        if(!distanceMeasurement_.getResult()){
          straightRunning_.run(50, 100);
        }else{
          straightRunning_.initialize();
          Status_ = Status::STRAIGHT_SPEED_DOWN;
        }
        break;

      case Status::STRAIGHT_SPEED_DOWN:
        //ライン検知するまで
        if(!lineDetection_.getResult()){
          straightRunning_.run(20, 100);
        }else{
          //信地旋回準備
          bodyAngleMeasurement_.setBaseAngle();
          Status_ = Status::TURN;
        }
        break;

      case Status::TURN:
        //90°信地旋回
        if(bodyAngleMeasurement_.getResult() <= -85){
          Status_ = Status::LINETRACE;
        }else{
          curveRunning_.run(0, 30);
        }
        break;

      case Status::LINETRACE:
        linetrace_->run(20,drive::LineTraceEdge::LEFT,0.6);
        return true;

    }
    return false;
  }
};
