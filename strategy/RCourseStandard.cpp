#include "RCourseStandard.h"

using namespace drive;
using namespace measurement;

namespace strategy{
    RCourseStandard::RCourseStandard(){
        linetrace_ = LineTrace::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        bodyAngleMeasurement_ = BodyAngleMeasurement();
        curveRunning_ = new CurveRunning();

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

        case Phase::INIT:
            distanceMeasurement_->start(7000);
            linetrace_->setPid(LineTracePid::MID);//VERY_FAST

        case Phase::STRAIGHT1:
            linetrace_->run(40,LineTraceEdge::LEFT);//runで速度渡してる
            if(distanceMeasurement_->getResult()){
                linetrace_->run(0,LineTraceEdge::LEFT);
            }
            return false;

        case Phase::BEND1:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(80);
            linetrace_->setEdge(LineTraceEdge::RIGHT);
            return fixedDistanceCurveLineTrace(3100,-10);

        case Phase::BEND2:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(80);
            linetrace_->setEdge(LineTraceEdge::RIGHT);
            return fixedDistanceCurveLineTrace(700,100);

        case Phase::BEND3:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(80);
            linetrace_->setEdge(LineTraceEdge::RIGHT);
            return fixedDistanceCurveLineTrace(1100,1950);

        case Phase::BEND4:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(80);
            linetrace_->setEdge(LineTraceEdge::RIGHT);
            return fixedDistanceCurveLineTrace(1250,-20);

        case Phase::STRAIGHT2:
            linetrace_->setPid(LineTracePid::VERY_FAST);
            return fixedDistanceLineTrace(1730,80,LineTraceEdge::RIGHT);

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
