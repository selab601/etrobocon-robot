#include "BlockAreaEntry.h"


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

    Status_ = Status::STANDBY;

  }

  bool BlockAreaEntry::capture(){
    switch(Status_){
      case Status::STANDBY:
        //侵入場所の真横まで
        distanceMeasurement_.setTargetDistance(550);
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
        if(pivotTurn_.turn(90)){
          Status_ = Status::STRAIGHT_RUN;
        }
        break;

      case Status::STRAIGHT_RUN:
        //ライン検知するまで
        if(!lineDetection_.getResult()){
          straightRunning_.run(30);
        }else{
          //信地旋回準備
          bodyAngleMeasurement_.setBaseAngle();
          Status_ = Status::TURN2;
        }
        break;

      case Status::TURN2:
        //90°信地旋回
        if(bodyAngleMeasurement_.getResult() >= 85){
          Status_ = Status::LINETRACE2;
        }else{
          curveRunning_.run(30,-5);
        }
        break;

      case Status::LINETRACE2:
        //色検知するまで
        if(colorDetection_.getResult() == COLOR_RED || colorDetection_.getResult() == COLOR_YELLOW || colorDetection_.getResult() == COLOR_GREEN || colorDetection_.getResult() == COLOR_BLUE){
          motor_->setWheelPWM(0,0);
          distanceMeasurement_.setTargetDistance(140);
          distanceMeasurement_.startMeasurement();
          Status_ = Status::BACK;
        }else{
          linetrace_->setPid(0.006F, 0.0F, 0.52F);
          linetrace_->run(20,drive::LineTraceEdge::RIGHT,0.3);
        }
        break;

      case Status::BACK:
        if(!distanceMeasurement_.getResult()){
          straightRunning_.run(-10);
        }else{
          Status_ = Status::DONE;
        }
        break; 

      case Status::DONE:
        motor_->setWheelPWM(0,0);
        return true;
    }
    return false;
  }
};
