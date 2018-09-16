#include "RCourseStandard.h"

using namespace drive;
using namespace measurement;

namespace strategy{
    RCourseStandard::RCourseStandard(){
        linetrace_ = LineTrace::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        bodyAngleMeasurement_ = BodyAngleMeasurement();
        curveRunning_ = new CurveRunning();
        straightRunning_ = new StraightRunning();
        //motor_ = Motors::getInstance();

        strategySuccess_ = false;
        hasExecutedPhase_ = false;
        isLineTraceReset_ = false;
    }

    bool RCourseStandard::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            if(executePhase(phaseProcedure_[procedureNumber])){
                lineTraceReset();
                distanceMeasurement_->reset();
                procedureNumber++;
                hasExecutedPhase_ = false;
                ev3_speaker_set_volume(6);
                ev3_speaker_play_tone(500,100);
            }
        }
        if(procedureNumber == phaseProcedure_.size()){
            strategySuccess_ = true;
        }
        return strategySuccess_;
    }

    bool RCourseStandard::executePhase(Phase phase){
        switch(phase){

        case Phase::STRAIGHT1:
            startDistanceMeasurement(2056);
            linetrace_->setPid(LineTracePid::VERY_FAST);
            linetrace_->run(80,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case Phase::BEND1:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(70);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(1850,230);

        case Phase::STRAIGHT2:
            startDistanceMeasurement(450);
            linetrace_->setPid(LineTracePid::VERY_FAST);
            linetrace_->run(75,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case Phase::BEND2:
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setMaxPwm(65);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(645,450);

        case Phase::BEND3:
            linetrace_->setPid(LineTracePid::MID);//改良の余地あり　感度強く、遅くMID
            linetrace_->setMaxPwm(45);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(650,-480); //690

        case Phase::STRAIGHT3:
            startDistanceMeasurement(550); //同じくいじるか否かMID
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->run(50,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case Phase::BEND4:
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setMaxPwm(50);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(1250,-480);

        case Phase::STRAIGHT4:
            startDistanceMeasurement(2600);
            linetrace_->setPid(LineTracePid::VERY_FAST);
            linetrace_->run(80,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case Phase::BEND5:
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setTarget(0.7);
            linetrace_->setMaxPwm(50);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(470,700);//520,800

        case Phase::STRAIGHT5:
            startDistanceMeasurement(1850);//380
            linetrace_->setPid(LineTracePid::RETURN);
            linetrace_->run(30,LineTraceEdge::LEFT,0.7);
            return distanceMeasurement_->getResult();

        case Phase::BEND6:
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setMaxPwm(450);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(2000,600);

        case Phase::STOP:
            linetrace_->run(0,LineTraceEdge::RIGHT);
            return false;
        default: return false;
        }
    }

    void RCourseStandard::startDistanceMeasurement(int distance){
        if(!hasExecutedPhase_){
            distanceMeasurement_->setTarget(distance);
            distanceMeasurement_->start();
            hasExecutedPhase_ = true;
        }
    }

    void RCourseStandard::startAngleMeasurement(){
        if(!hasExecutedPhase_){
            bodyAngleMeasurement_.setBaseAngle();
            hasExecutedPhase_ = true;
        }
    }


    bool RCourseStandard::fixedDistanceLineTrace(int distance,int speed,LineTraceEdge edge){
        startDistanceMeasurement(distance);
        linetrace_->run(speed,edge);
        return distanceMeasurement_->getResult();
    }

    bool RCourseStandard::fixedDistanceCurveLineTrace(int distance,int deltaRad){
        startDistanceMeasurement(distance);
        linetrace_->runCurve(deltaRad);
        return distanceMeasurement_->getResult();
    }


    void RCourseStandard::lineTraceReset(){
        if(!isLineTraceReset_){
            linetrace_->reset();
            isLineTraceReset_ = true;
        }
    }

};
