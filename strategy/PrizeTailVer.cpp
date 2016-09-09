#include "PrizeTailVer.h"

using namespace drive;
using namespace detection;
using namespace measurement;
using namespace device;

namespace strategy{
    PrizeTailVer::PrizeTailVer(){
        lineTrace_ = LineTrace::getInstance();
        straightRunning_ = new StraightRunning();
        pivotTurn_ = new PivotTurn();
        curveRunning_ = new CurveRunning();
        objectDetection_ = new ObjectDetection(100);
        lineDetection_ = new LineDetection();
        distanceMeasurement_  = new DistanceMeasurement();
        timeMeasurement_ = new TimeMeasurement();
        bodyAngleMeasurement_ = new BodyAngleMeasurement();
        motors_ = Motors::getInstance();
        strategySuccess_ = false;
        hasExecutedPhase_ = false;
    }

    bool PrizeTailVer::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            if(executePhase(phaseProcedure_[procedureNumber])){
                procedureNumber++;
                hasExecutedPhase_ = false;
                bodyAngleMeasurement_->setBaseAngle();
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        if(procedureNumber == phaseProcedure_.size()){
            strategySuccess_ = true;
        }
        return strategySuccess_;
    }

    bool PrizeTailVer::executePhase(Phase phase){
        static int baceCount = 0;
        switch(phase){
        case Phase::LINE_TRACE:
            lineTrace_->run(15,LineTraceEdge::RIGHT);
            baceCount = motors_->getCount(MOTOR_ARM);
            return objectDetection_->getResult();

        case Phase::DOWN_ARM:
            startTimeMeasurement(500);
            motors_->setPWM(MOTOR_ARM, -20);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        case Phase::PUT_IN_LOWER:
            startDistanceMeasurement(80);
            motors_->setPWM(MOTOR_ARM, 0);
            straightRunning_->run(10);
            return distanceMeasurement_->getResult();

        case Phase::LIFT:
            startTimeMeasurement(1000);
            motors_->setPWM(MOTOR_ARM, 30);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        case Phase::DOWN:
            motors_->setPWM(MOTOR_ARM,-20);
            straightRunning_->run(0);
            return motors_->getCount(MOTOR_ARM) <= baceCount;

        case Phase::BACK:
            startDistanceMeasurement(50);
            straightRunning_->run(-10);
            return distanceMeasurement_->getResult();

        case Phase::BACK2:
            startDistanceMeasurement(35);
            straightRunning_->run(-10);
            return distanceMeasurement_->getResult();

        case Phase::TURN:
            return pivotTurn_->turn(90);

        case Phase::CURVE:
            curveRunning_->run(10,30);
            return bodyAngleMeasurement_->getResult() <= -90;

        case Phase::TAIL_MOVE:
            startTimeMeasurement(500);
            motors_->setPWM(MOTOR_TAIL,-50);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        case Phase::STRAIGHT_1_CM:
            startDistanceMeasurement(20);
            straightRunning_->run(10);
            return distanceMeasurement_->getResult();

        case Phase::ZERO:
            motors_->setPWM(MOTOR_TAIL,0);
            return true;

        default: return false;
        }
    }

    void PrizeTailVer::startTimeMeasurement(int time){
        if(!hasExecutedPhase_){
            timeMeasurement_->setBaseTime();
            timeMeasurement_->setTargetTime(time);
            hasExecutedPhase_ = true;
        }
    }

    void PrizeTailVer::startDistanceMeasurement(int distance){
        if(!hasExecutedPhase_){
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            hasExecutedPhase_ = true;
        }
    }

}
