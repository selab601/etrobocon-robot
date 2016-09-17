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
          ev3_speaker_set_volume(7);
        ev3_speaker_play_tone(500,200);
        linetrace_->reset();
        linetrace_->setPid();
        bodyAngleMeasurement_.setBaseAngle();
        Status_ = Status::DETECTION_CURVE;
        break;

      //スピード競技前の90°カーブを検知
      case Status::DETECTION_CURVE:
        //HIYOKO -85
        //KOTORI -83
        //SELAB  -82
        //KAKERUN -85
        if(bodyAngleMeasurement_.getResult() <= -90){
          Status_ = Status::STRAIGHT1_STANDBY;
        }else{
          linetrace_->run(20,drive::LineTraceEdge::LEFT,0.7);
        }

        break;

      case Status::STRAIGHT1_STANDBY:
        ev3_speaker_play_tone(500,200);
      	// 直線(1300)，カーブ()　直径(480)，直線()
        linetrace_->reset();
        linetrace_->setPid(0.003F, 0.0, 0.3F);
        //HIYOKO(1250)
        //KOTORI(1250)
        //SELAB(1230)
        //KAKERUN(1250)
        distanceMeasurement_.setTargetDistance(1300);
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
        ev3_speaker_play_tone(500,200);
        //HIYOKO(744)
        //KOTORI(744)
        //SELAB(730)
        //KAKERUN(715)
        distanceMeasurement_.setTargetDistance(650); //550
        distanceMeasurement_.startMeasurement();

        // カーブ用ライントレース
        linetrace_->reset();
        linetrace_->setTarget(0.5);
        linetrace_->setEdge(drive::LineTraceEdge::LEFT);
        linetrace_->setPid(0.005,0,0.6);
        linetrace_->setMaxPwm(60);
        Status_ = Status::CURVE1;
        break;

      case Status::CURVE1:
        if(!distanceMeasurement_.getResult()){
          linetrace_->runCurve(750); // 750でカーブいい感じ
        }else{
          Status_ = Status::STRAIGHT2_STANDBY;
        }
        break;

      case Status::STRAIGHT2_STANDBY:
        ev3_speaker_play_tone(500,200);
        linetrace_->reset();
        //直線2
        //HIYOKO 1350
        //SELAB 1250
        distanceMeasurement_.setTargetDistance(1250); //1350
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
        ev3_speaker_play_tone(500,200);
        //840だったけど多いかも
        distanceMeasurement_.setTargetDistance(820);
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.0144,0.0,0.72);
        Status_ = Status::CURVE2;
        break;

      case Status::CURVE2:
        if(!distanceMeasurement_.getResult()){
          //HIYOKO(34,58)
          //KOTORI(35,58)
          //SELAB(35,58)  電池 (33,60)
          //KAKERUN(31,61
          curveRunning_.run(33, 60);    // TODO: 機体に応じて変更
        }else{
          Status_ = Status::STRAIGHT3_STANDBY;
        }
        break;

      case Status::STRAIGHT3_STANDBY:
        ev3_speaker_play_tone(500,200);
        linetrace_->reset();
        //直線2
        //KAKERUN(1550)
        distanceMeasurement_.setTargetDistance(1700);//1600
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
        ev3_speaker_play_tone(500,200);
        distanceMeasurement_.setTargetDistance(430);
        distanceMeasurement_.startMeasurement();

        linetrace_->reset();
        linetrace_->setPid(0.005,0,0.6);
        linetrace_->setTarget(0.5);
        linetrace_->setEdge(drive::LineTraceEdge::LEFT);
        linetrace_->setMaxPwm(60);
        Status_ = Status::CURVE3;
        break;

      case Status::CURVE3:
        if(!distanceMeasurement_.getResult()){
          //linetrace_->setPid(0.006F, 0.0F, 0.52F);
            linetrace_->runCurve(-750); // 750でカーブいい感じ
        }else{
          Status_ = Status::STRAIGHT4_STANDBY;
        }
        break;

      case Status::STRAIGHT4_STANDBY:
        ev3_speaker_play_tone(500,200);
        ev3_speaker_set_volume(1);
        //ゴールまで
        distanceMeasurement_.setTargetDistance(2700); //2770
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.003F, 0.0, 0.3F);
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
