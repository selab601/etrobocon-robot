#include "PrizeTailVer.h"

using namespace drive;
using namespace detection;
using namespace measurement;
using namespace device;

namespace strategy{
    PrizeTailVer::PrizeTailVer(){
        lineTrace_              = LineTrace::getInstance();
        straightRunning_        = new StraightRunning();
        pivotTurn_              = new PivotTurn();
        curveRunning_           = new CurveRunning();
        objectDetection_        = new ObjectDetection(100);
        lineDetection_          = new LineDetection();
        distanceMeasurement_    = new DistanceMeasurement();
        timeMeasurement_        = new TimeMeasurement();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();
        motors_                 = Motors::getInstance();
        strategySuccess_        = false;
        hasExecutedPhase_       = false;
        isLineTraceReset_       = false;
    }

    //攻略する
    bool PrizeTailVer::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            //難所攻略手順を1つする実行する
            if(executePhase(phaseProcedure_[procedureNumber])){
                lineTraceReset();
                procedureNumber++;
                hasExecutedPhase_ = false;//フラグを戻しておく
                bodyAngleMeasurement_->setBaseAngle();//車体角度リセット
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        //最後まで終わったら
        if(procedureNumber == phaseProcedure_.size()){
            strategySuccess_ = true;
        }
        return strategySuccess_;
    }

    //攻略手順
    bool PrizeTailVer::executePhase(Phase phase){
        switch(phase){

        //ライントレース
        case Phase::LINE_TRACE:
            lineTraceReset();
            startDistanceMeasurement(1000);
            lineTrace_->setPid();
            lineTrace_->run(70,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case Phase::LINE_TRACE2:
            lineTraceReset();
            lineTrace_->setPid(0.003,0.0,0.3);
            lineTrace_->run(40,LineTraceEdge::RIGHT);//ここのスピード

            if (device::SonarSensor::getInstance()->getDistance() <= (5 + 15)){
                return true;
            }
            return false;

        //アームを下げる
        case Phase::DOWN_ARM:
            lineTrace_->setPid();
            lineTrace_->setTarget(0.4);
            return Arm::getInstance()->down();

        case Phase::LINE_TRACE_UP_TO_PRIZE:
            lineTrace_->run(15,LineTraceEdge::RIGHT);
            if (device::SonarSensor::getInstance()->getDistance() <= 5){
                straightRunning_->run(0);
                return true;
            }
            return false;

        case Phase::TURN_LEFT:
            return pivotTurn_->turn(5, 10);

        //懸賞の下にアームを入れる
        case Phase::PUT_IN_LOWER_OF_PRIZE:
            startDistanceMeasurement(50);
            straightRunning_->run(10);
            return distanceMeasurement_->getResult();

        //持ち上げる
        case Phase::LIFT_PRIZE:
            startTimeMeasurement(1000);
            straightRunning_->run(0);
            return Arm::getInstance()->up(15) || timeMeasurement_->getResult();

        case Phase::BACK_14CM:
            startDistanceMeasurement(125);//ここの距離確認(ラインの右側にいるかどうか)
            straightRunning_->run(-10);
            return distanceMeasurement_->getResult();

        //下ろす
        case Phase::DOWN_PRIZE:
            straightRunning_->run(0);
            return Arm::getInstance()->normal(15);

        case Phase::DOWN_SHIPPO:
            return Shippo::getInstance()->bored();

        //すこし下がる
        case Phase::PUT_AFTER_BACK:
            startDistanceMeasurement(50);
            straightRunning_->run(-10);
            return distanceMeasurement_->getResult();

        //
        case Phase::CURVE_AFTER_BACK:
            startDistanceMeasurement(35);
            straightRunning_->run(-10);
            return distanceMeasurement_->getResult();

        //左に90度旋回
        case Phase::LEFT_90_ROTATION:
            return pivotTurn_->turn(90,30);

        case Phase::LEFT_90_ROTATION_SLOWLY:
            return pivotTurn_->turn(90,10);

        //懸賞の左側にカーブで移動
        case Phase::CURVE_UP_TO_PRIZE_SIDE:
            curveRunning_->run(12,30);
            return bodyAngleMeasurement_->getResult() <= -90;

        //尻尾を動かす
        case Phase::GET_PRIZE:
            return Shippo::getInstance()->getPrize();

        //1cm進む
        case Phase::STRAIGHT_1_CM:
            startDistanceMeasurement(20);
            straightRunning_->run(10);
            if ( distanceMeasurement_->getResult()){
                straightRunning_->run(0);
                return true;
            }
            return false;

        case Phase::RIGHT_10_ROTATION:
            return pivotTurn_->turn(-10,20);

        //ゴールまでライントレース
        case Phase::LINE_TRACE_UP_TO_GOOL:
            //startDistanceMeasurement(350);//カーブ用
            startDistanceMeasurement(500);
            lineTrace_->setPid(0.0144,0.0,0.72);
            // lineTrace_->setEdge(LineTraceEdge::RIGHT);
            // lineTrace_->setMaxPwm(60);
            // lineTrace_->runCurve(1080);
            lineTrace_->run(40,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case Phase::LINE_TRACE_UP_TO_GOOL2:
            //startDistanceMeasurement(2850);//カーブ用
            startDistanceMeasurement(2700);
            lineTrace_->setPid();
            lineTrace_->run(80,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case Phase::FINISHED:
            straightRunning_->run(0);
            //尻尾を振らなければ次のテスト時に尻尾を合わせなくて良くなる
            //Shippo::getInstance()->furifuri();
            return false;

        default: return false;
        }
    }

    //時間検知の設定をまとめたもの
    void PrizeTailVer::startTimeMeasurement(int time){
        if(!hasExecutedPhase_){
            timeMeasurement_->setBaseTime();
            timeMeasurement_->setTargetTime(time);
            hasExecutedPhase_ = true;
        }
    }

    //距離検知の設定をまとめたもの
    void PrizeTailVer::startDistanceMeasurement(int distance){
        if(!hasExecutedPhase_){
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            hasExecutedPhase_ = true;
        }
    }

    void PrizeTailVer::lineTraceReset(){
        if(!isLineTraceReset_){
            lineTrace_->reset();
            isLineTraceReset_ = true;
        }
    }

}
