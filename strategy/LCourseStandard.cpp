#include "LCourseStandard.h"


namespace strategy{
  LCourseStandard::LCourseStandard(){
    linetrace_ = drive::LineTrace::getInstance();
    distanceMeasurement_ = measurement::DistanceMeasurement();
    motor_ = device::Motors::getInstance();
    Status_ = Status::STANDBY;

  }

  bool LCourseStandard::capture(){
    switch(Status_){
      case Status::STANDBY:
        //エッジ切り替え地点まで 
        //最初のストレート:2500
        //カーブ:500
        distanceMeasurement_.setTargetDistance(6000);
        distanceMeasurement_.startMeasurement();
        Status_ = Status::LINETRACE1;
        break;
      
      case Status::LINETRACE1:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(80,drive::LineTraceEdge::RIGHT,0.6);
        }else{
        	//エッジ切り替え準備

          Status_ = Status::CHANGEEDGE_TO_LEFT;
        }
        break;

      case Status::CHANGEEDGE_TO_LEFT:
      	//右から左へ
        if(linetrace_->changeEdge()){
          //ライントレース準備
          //少し進んでから右エッジに変更
          distanceMeasurement_.setTargetDistance(1000);
          distanceMeasurement_.startMeasurement();
          Status_ = Status::LINETRACE2;
        }
        break;

      case Status::LINETRACE2:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(80,drive::LineTraceEdge::LEFT,0.6);
        }else{
          //エッジ切り替え準備
          Status_ = Status::CHANGEEDGE_TO_RIGHT;
        }
        break;

      case Status::CHANGEEDGE_TO_RIGHT:
        //右から左へ
        if(linetrace_->changeEdge()){
          //ライントレース準備
          //星取り前の分岐まで
          distanceMeasurement_.setTargetDistance(1000);
          distanceMeasurement_.startMeasurement();
          Status_ = Status::LINETRACE3;
        }
        break;

      case Status::LINETRACE3:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(80,drive::LineTraceEdge::RIGHT,0.6);
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
