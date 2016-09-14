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
        //HIYOKO -85
        //KOTORI -83
        //SELAB  -82
        //KAKERUN -85
        if(bodyAngleMeasurement_.getResult() <= -85){
          Status_ = Status::STRAIGHT1_STANDBY;
        }else{
          linetrace_->run(20,drive::LineTraceEdge::LEFT,0.6);
        }

        break;

      case Status::STRAIGHT1_STANDBY:
      	// 直線(1300)，カーブ()　直径(480)，直線()
        linetrace_->setPid(0.003F, 0.0, 0.3F);
        //HIYOKO(1250)
        //KOTORI(1250)
        //SELAB(1230)
        //KAKERUN(1250)
        distanceMeasurement_.setTargetDistance(1250);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::STRAIGHT1;
        break;
      
      case Status::STRAIGHT1:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(100,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::CURVE1_STANDBY;
        }
        break;

      //第一カーブ部分
      case Status::CURVE1_STANDBY:
        //HIYOKO(744)
        //KOTORI(744)
        //SELAB(730)
        //KAKERUN(715)
        distanceMeasurement_.setTargetDistance(715);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::CURVE1;
        break;

      case Status::CURVE1:
        if(!distanceMeasurement_.getResult()){
          //HIYOKO(56,32)
          //KOTORI(56,32)
          //SELAB(56,32)
          //KAKERUN(63,31)
          curveRunning_.run(63, 31);
        }else{
          Status_ = Status::STRAIGHT2_STANDBY;
        }
        break;

      case Status::STRAIGHT2_STANDBY:
        //直線2
        distanceMeasurement_.setTargetDistance(1150);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::STRAIGHT2;
        break;

      case Status::STRAIGHT2:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(100,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::CURVE2_STANDBY;
        }
        break;

      //直角に近いカーブ部分
      case Status::CURVE2_STANDBY:
        distanceMeasurement_.setTargetDistance(840);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::CURVE2;
        break;

      case Status::CURVE2:
        if(!distanceMeasurement_.getResult()){
          //HIYOKO(34,58)
          //KOTORI(35,58)
          //SELAB(35,58)
          //KAKERUN(31,61) 
          curveRunning_.run(31, 61);
        }else{
          Status_ = Status::STRAIGHT3_STANDBY;
        }
        break;

      case Status::STRAIGHT3_STANDBY:
        //直線2
        //KAKERUN(1550)
        distanceMeasurement_.setTargetDistance(1600);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::STRAIGHT3;
        break;

      case Status::STRAIGHT3:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(100,drive::LineTraceEdge::LEFT,0.6);
        }else{
          Status_ = Status::CURVE3_STANDBY;
        }
        break;

      //最後の直角カーブ部分
      case Status::CURVE3_STANDBY:
        distanceMeasurement_.setTargetDistance(430);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::CURVE3;
        break;

      case Status::CURVE3:
        if(!distanceMeasurement_.getResult()){
          //linetrace_->setPid(0.006F, 0.0F, 0.52F);
          //linetrace_->run(40,drive::LineTraceEdge::LEFT,0.6);
          //
          //HIYOKO(39,60)
          //KOTORI(39,60)
          //SELAB(39,62)
          //KAKERUN(35,63) 失敗
          curveRunning_.run(35, 63);
        }else{
          Status_ = Status::STRAIGHT4_STANDBY;
        }
        break;

      case Status::STRAIGHT4_STANDBY:
        //ゴールまで
        distanceMeasurement_.setTargetDistance(2770);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.003F, 0.0, 0.3F);
        Status_ = Status::STRAIGHT4;
        break;

      case Status::STRAIGHT4:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(100,drive::LineTraceEdge::LEFT,0.6);
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
