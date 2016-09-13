#include "RCourseStandard.h"
#include "device/Shippo.h"


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
        if(bodyAngleMeasurement_.getResult() <= -80){
          Status_ = Status::STRAIGHT1_STANDBY;
        }else{
          linetrace_->run(20,drive::LineTraceEdge::LEFT,0.6);
        }

        break;

      case Status::STRAIGHT1_STANDBY:
      	// 直線(1300)，カーブ()直径(480)，直線()
        linetrace_->setPid(0.003F, 0.0F, 0.3F);
        distanceMeasurement_.setTargetDistance(1250);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::STRAIGHT1;
        break;

      case Status::STRAIGHT1:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::CURVE1_STANDBY;
        }
        break;

      //第一カーブ部分
      case Status::CURVE1_STANDBY:
        distanceMeasurement_.setTargetDistance(800);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.0144,0.0,0.72);
        Status_ = Status::CURVE1;
        break;

      case Status::CURVE1:
        if(!distanceMeasurement_.getResult()){
          //成功(56,32)
          //curveRunning_.run(56, 32);
            linetrace_->run(50,drive::LineTraceEdge::LEFT);
        }else{
          Status_ = Status::STRAIGHT2_STANDBY;
        }
        break;

      case Status::STRAIGHT2_STANDBY:
        //直線2
        distanceMeasurement_.setTargetDistance(1200);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.003F, 0.0F, 0.3F);
        Status_ = Status::STRAIGHT2;
        break;

      case Status::STRAIGHT2:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::CURVE2_STANDBY;
        }
        break;

      //直角に近いカーブ部分
      case Status::CURVE2_STANDBY:
        distanceMeasurement_.setTargetDistance(1200);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.0144,0.0,0.72);
        Status_ = Status::CURVE2;
        break;

      case Status::CURVE2:
        if(!distanceMeasurement_.getResult()){
            //curveRunning_.run(34, 58);
            linetrace_->run(30,drive::LineTraceEdge::LEFT);
        }else{
          Status_ = Status::STRAIGHT3_STANDBY;
        }
        break;

      case Status::STRAIGHT3_STANDBY:
        //ゴールまで
        distanceMeasurement_.setTargetDistance(1600);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.003F, 0.0F, 0.3F);
        Status_ = Status::STRAIGHT3;
        break;

      case Status::STRAIGHT3:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::CURVE3_STANDBY;
        }
        break;

      //最後の直角カーブ部分
      case Status::CURVE3_STANDBY:
        distanceMeasurement_.setTargetDistance(500);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.0144,0.0,0.72);
        Status_ = Status::CURVE3;
        break;

      case Status::CURVE3:
        if(!distanceMeasurement_.getResult()){
          //linetrace_->setPid(0.006F, 0.0F, 0.52F);
          //linetrace_->run(40,drive::LineTraceEdge::LEFT,0.6);
          //curveRunning_.run(39, 60);
            linetrace_->run(50,drive::LineTraceEdge::LEFT);
        }else{
          Status_ = Status::STRAIGHT4_STANDBY;
        }
        break;

      case Status::STRAIGHT4_STANDBY:
        //ゴールまで
        distanceMeasurement_.setTargetDistance(2770);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.003F, 0.0F, 0.3F);
        Status_ = Status::STRAIGHT4;
        break;

      case Status::STRAIGHT4:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::DONE;
        }
        break;

      case Status::DONE:
        motor_->setWheelPWM(0,0);
        device::Shippo::getInstance()->furifuri();  // 終わったらしっぽふりふり
        return true;
    }
    return false;
  }
};
