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
    }

    //攻略する
    bool PrizeTailVer::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            //難所攻略手順を1つする実行する
            if(executePhase(phaseProcedure_[procedureNumber])){
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
        static int baceCount = 0;
        switch(phase){

        //ライントレースしながら障害物検知
        case Phase::LINE_TRACE_UP_TO_PRIZE:
            lineTrace_->run(15,LineTraceEdge::RIGHT);
            baceCount = motors_->getCount(MOTOR_ARM);
            return objectDetection_->getResult();

        //アームを下げる
        case Phase::DOWN_ARM:
            startTimeMeasurement(500);
            motors_->setPWM(MOTOR_ARM, -20);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //懸賞の下にアームを入れる
        case Phase::PUT_IN_LOWER_OF_PRIZE:
            startDistanceMeasurement(80);
            motors_->setPWM(MOTOR_ARM, 0);
            straightRunning_->run(10);
            return distanceMeasurement_->getResult();

        //持ち上げる
        case Phase::LIFT_PRIZE:
            startTimeMeasurement(1000);
            motors_->setPWM(MOTOR_ARM, 30);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //下ろす
        case Phase::DOWN_PRIZE:
            motors_->setPWM(MOTOR_ARM,-20);
            straightRunning_->run(0);
            return motors_->getCount(MOTOR_ARM) <= baceCount;

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
            return pivotTurn_->turn(90);

        //懸賞の左側にカーブで移動
        case Phase::CURVE_UP_TO_PRIZE_SIDE:
            curveRunning_->run(10,30);
            return bodyAngleMeasurement_->getResult() <= -90;

        //尻尾を動かす
        case Phase::SPIN_TAIL:
            startTimeMeasurement(500);
            motors_->setPWM(MOTOR_TAIL,-50);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //1cm進む
        case Phase::STRAIGHT_1_CM:
            startDistanceMeasurement(20);
            straightRunning_->run(10);
            return distanceMeasurement_->getResult();

        //尻尾を止める
        case Phase::STOP_TAIL:
            motors_->setPWM(MOTOR_TAIL,0);
            return true;

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

}
