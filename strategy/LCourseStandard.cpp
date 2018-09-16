#include "LCourseStandard.h"

using namespace drive;
using namespace measurement;

namespace strategy{
    LCourseStandard::LCourseStandard(){
        linetrace_ = LineTrace::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        bodyAngleMeasurement_ = BodyAngleMeasurement();
        curveRunning_ = new CurveRunning();

        strategySuccess_ = false;
        hasExecutedPhase_ = false;
        isLineTraceReset_ = false;
    }

    bool LCourseStandard::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            if(executePhase(phaseProcedure_[procedureNumber])){
                lineTraceReset();
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

    bool LCourseStandard::executePhase(Phase phase){
        switch(phase){

        case Phase::LINETRACE1:
            startDistanceMeasurement(2620);
            lineTraceReset();

            linetrace_->setPid(LineTracePid::VERY_FAST);
            linetrace_->run(80,LineTraceEdge::LEFT);

            return distanceMeasurement_->getResult();

        case Phase::CURVE1:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(70);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(1600,100);

        case Phase::LINETRACE2:
            linetrace_->setPid(LineTracePid::VERY_FAST);
            return fixedDistanceLineTrace(1455,80,LineTraceEdge::LEFT);

        case Phase::CURVE2:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(70);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(780,-403);

        case Phase::LINETRACE3:
            linetrace_->setPid(LineTracePid::VERY_FAST);
            return fixedDistanceLineTrace(500,80,LineTraceEdge::LEFT);

        case Phase::CURVE3:
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setMaxPwm(50);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(700,-300);

        case Phase::LINETRACE4:
            linetrace_->setPid(LineTracePid::VERY_FAST);
            return fixedDistanceLineTrace(2700,80,LineTraceEdge::LEFT);//goalまで

        case Phase::CURVE4:
            linetrace_->setPid(LineTracePid::FAST);
            linetrace_->setMaxPwm(80);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(1000,400);

        case Phase::CURVE5:
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setMaxPwm(80);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(950,-100);

        case Phase::CHANGEEDGE_L_R:
            startDistanceMeasurement(100);
            curveRunning_->run(48,60);
            return distanceMeasurement_->getResult();

        case Phase::LINETRACE5:
            linetrace_->setPid(LineTracePid::FAST);
            return fixedDistanceLineTrace(900,60,LineTraceEdge::RIGHT);

        default:
            return false;
        }
    }

    void LCourseStandard::startDistanceMeasurement(int distance){
        if(!hasExecutedPhase_){
            distanceMeasurement_->setTarget(distance);
            distanceMeasurement_->start();
            hasExecutedPhase_ = true;
        }
    }

    void LCourseStandard::startAngleMeasurement(){
        if(!hasExecutedPhase_){
            bodyAngleMeasurement_.setBaseAngle();
            hasExecutedPhase_ = true;
        }
    }


    bool LCourseStandard::fixedDistanceLineTrace(int distance,int speed,LineTraceEdge edge){
        startDistanceMeasurement(distance);
        linetrace_->run(speed,edge);
        return distanceMeasurement_->getResult();
    }

    bool LCourseStandard::fixedDistanceCurveLineTrace(int distance,int deltaRad){
        startDistanceMeasurement(distance);
        linetrace_->runCurve(deltaRad);
        return distanceMeasurement_->getResult();
    }


    void LCourseStandard::lineTraceReset(){
        if(!isLineTraceReset_){
            linetrace_->reset();
            isLineTraceReset_ = true;
        }
    }
};
