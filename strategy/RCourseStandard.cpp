#include "RCourseStandard.h"
#include "device/Shippo.h"
#include "../device/Display.h"


namespace strategy{
  RCourseStandard::RCourseStandard(){
    linetrace_ = drive::LineTrace::getInstance();
    distanceMeasurement_ = measurement::DistanceMeasurement();
    motor_ = device::Motors::getInstance();
    Status_ = Status::DETECTION_STANDBY;
    bodyAngleMeasurement_ = measurement::BodyAngleMeasurement();
    straightRunning_ = drive::StraightRunning();
    lineDetection_ = detection::LineDetection();
    pivotTurn_ = drive::PivotTurn();
  }

  bool RCourseStandard::capture(){
      static long baseLength = 0;
      static long targetLength = 0;
      static int angleCounter = 0;
      static int angle[5];
      static int baseAngle = 0;
      SelfPositionEstimation* estimation = SelfPositionEstimation::getInstance();

    switch(Status_){
      case Status::DETECTION_STANDBY:
          ev3_speaker_set_volume(7);
        ev3_speaker_play_tone(500,200);
        linetrace_->reset();
        linetrace_->setPid(0.018, 0, 1.2);
        bodyAngleMeasurement_.setBaseAngle();

        baseLength = estimation->getMigrationLength();
        targetLength = 0;
        angleCounter = 0;
        Status_ = Status::DETECTION_CURVE;
        break;

      //スピード競技前の90°カーブを検知
      case Status::DETECTION_CURVE:
        //HIYOKO -85
        //KOTORI -83
        //SELAB  -82
        //KAKERUN -85
        if(bodyAngleMeasurement_.getResult() <= -80){ // -90 だと検知しないことがある
            angleCounter = 0;
          Status_ = Status::STRAIGHT1_STANDBY;
        }else{
          linetrace_->run(50,drive::LineTraceEdge::LEFT, 0.5);
        }

        if (estimation->getMigrationLength() - baseLength >= targetLength){
            // 角度を測る
            if (angleCounter < 5 ){
                ev3_speaker_play_tone(500, 100);
                angle[angleCounter++] += estimation->getAngle();
                targetLength += 70;
            }
            else{
                // 5回計測した角度の平均をベースにセットする
                baseAngle =
                    (angle[0] + angle[1] + angle[2] + angle[3] + angle[4]) / 5;
            }
        }

        break;

      case Status::STRAIGHT1_STANDBY:
        ev3_speaker_play_tone(500,200);

        // ショートカット用
        baseLength = estimation->getMigrationLength();
        targetLength = 300;

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
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.5);
        }else{
          Status_ = Status::CURVE1_STANDBY;
        }

        // ショートカット用
        if (estimation->getMigrationLength() - baseLength >= targetLength){
            if (angleCounter < 5 ){
                ev3_speaker_play_tone(500, 100);
                angle[angleCounter++] += estimation->getAngle();
                targetLength += 100;
            }
        }
        break;

      //第一カーブ部分
      case Status::CURVE1_STANDBY:
        ev3_speaker_play_tone(500,200);

        // ショートカット用
        // 5回計測した角度の平均をベースにセットする
        baseAngle =
            (angle[0] + angle[1] + angle[2] + angle[3] + angle[4]) / 5;

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
        linetrace_->runCurve(800);

        // ショートカットするとき
        if (RCOURSE_SHORTCUT_FLAG != 0){
            if (estimation->getAngle() - baseAngle >= 70){
                Status_ = Status::TO_NEAR_GATE_LINE_CURVE;
            }
        }
        // ショートカットしないとき
        else{
            if(distanceMeasurement_.getResult()){
                Status_ = Status::STRAIGHT2_STANDBY;
            }
        }
        break;

      case Status::STRAIGHT2_STANDBY:
        ev3_speaker_play_tone(500,200);
        linetrace_->reset();
        //直線2
        //HIYOKO 1350
        //KOTORI 1250
        //SELAB 1250
        distanceMeasurement_.setTargetDistance(1350); //1350
        distanceMeasurement_.startMeasurement();
        linetrace_->setPid(0.003F, 0.0F, 0.3F);
        Status_ = Status::STRAIGHT2;
        break;

      case Status::STRAIGHT2:
        if(!distanceMeasurement_.getResult()){
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.5);
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
          //HIYOKO(34,58)  電池(35,59)
          //KOTORI(35,58) //電池(33,59)
          //SELAB(35,58)  電池 (33,60)
          //KAKERUN(31,61
          //curveRunning_.run(35, 59);    // TODO: 機体に応じて変更
          curveRunning_.run(35, 58);    // TODO: 機体に応じて変更
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
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.5);
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
            linetrace_->runCurve(-900);
        }else{
          Status_ = Status::STRAIGHT4_STANDBY;
        }
        break;

        // ショートカットVer
        // GATEのピンクのところあたりまでカーブで進む
      case Status::TO_NEAR_GATE_LINE_CURVE:
        curveRunning_.run(37, 58); // TODO: 調整必要かも
        if (estimation->getAngle() - baseAngle <= 30){
            Status_ = Status::TO_NEAR_GATE_LINE_STRAIGHT;

            distanceMeasurement_.startMeasurement();
            distanceMeasurement_.setTargetDistance(100);
            ev3_speaker_play_tone(500,200);
        }
        break;

        // ライン検知できるようにGATEのピンクのところ抜ける
      case Status::TO_NEAR_GATE_LINE_STRAIGHT:
        straightRunning_.run(35, 70);
        if (distanceMeasurement_.getResult()){
            Status_ = Status::LINE_DETECT;
        }
        break;

        // ライン検知するまで進む
      case Status::LINE_DETECT:
        straightRunning_.run(35);
        if(lineDetection_.getResult(30)){
            Status_ = Status::TO_GATE;
            ev3_speaker_play_tone(500,200);

            //distanceMeasurement_.startMeasurement();
            //distanceMeasurement_.setTargetDistance(200);
        }
        break;


      case Status::TO_GATE:
        //linetrace_->setPid(0.019, 0.0, 1.4);
        //linetrace_->run(25, drive::LineTraceEdge::RIGHT ,0.5);
        //curveRunning_.run(30, 5);
        curveRunning_.run(30, 2);

        if (estimation->getAngle() - baseAngle >= 80){
            ev3_speaker_play_tone(500,200);
            Status_ = Status::TURN;
        }
        break;

      case Status::TURN:
        if (pivotTurn_.turn(-170)){
            Status_ = Status::STRAIGHT4_STANDBY;
        }
        break;


        // 最後の直線(ショートカット・普通のやつ共通)
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
          linetrace_->run(70,drive::LineTraceEdge::LEFT,0.5);
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
