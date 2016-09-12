#include "RCourseStandard.h"


namespace strategy{
  RCourseStandard::RCourseStandard(){
    linetrace_ = drive::LineTrace::getInstance();
    distanceMeasurement_ = measurement::DistanceMeasurement();
    motor_ = device::Motors::getInstance();
    Status_ = Status::DETECTION_STANDBY;
    bodyAngleMeasurement_ = measurement::BodyAngleMeasurement();
  }

  bool RCourseStandard::capture(){
    switch(Status_){ 
      case Status::DETECTION_STANDBY:
        bodyAngleMeasurement_.setBaseAngle();
        Status_ = Status::DETECTION_CURVE;
        break;

      //スピード競技前の90°カーブを検知
      case Status::DETECTION_CURVE:
        if(bodyAngleMeasurement_.getResult() <= -85){
          Status_ = Status::STRAIGHT1_STANDBY;
        }else{
          linetrace_->run(20,drive::LineTraceEdge::LEFT,0.6);
        }

        break;

      case Status::STRAIGHT1_STANDBY:
      	// 直線，カーブ，直線
        distanceMeasurement_.setTargetDistance(3400);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::STRAIGHT1;
        break;
      
      case Status::STRAIGHT1:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(80,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::CURVE1_STANDBY;
        }
        break;

      //直角に近いカーブ部分
      case Status::CURVE1_STANDBY:
        distanceMeasurement_.setTargetDistance(800);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::CURVE1;
        break;

      case Status::CURVE1:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(30,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::STRAIGHT2_STANDBY;
        }
        break;

      case Status::STRAIGHT2_STANDBY:
        //ゴールまで
        distanceMeasurement_.setTargetDistance(4900);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::STRAIGHT2;
        break;

      case Status::STRAIGHT2:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(80,drive::LineTraceEdge::LEFT,0.6);
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
