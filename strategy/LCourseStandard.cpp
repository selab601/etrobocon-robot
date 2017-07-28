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
            startDistanceMeasurement(2180 - LCOURSE_SHORTCUT_LENGTH);
            lineTraceReset();
            linetrace_->setPid();
            linetrace_->run(100,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case Phase::CURVE1:
            linetrace_->setPid(0.006,0,0.4);
            //linetrace_->setPid(0, 0, 0);
            linetrace_->setMaxPwm(100);
            linetrace_->setEdge(LineTraceEdge::LEFT);

            //startAngleMeasurement();
            //linetrace_->runCurve(-1050);
            //return bodyAngleMeasurement_.getResult() <= -165;

            return fixedDistanceCurveLineTrace(1600,-2400);

        case Phase::CURVE2:
            linetrace_->setPid(0.006,0,0.4);
            linetrace_->setMaxPwm(100);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(900,0);


        case Phase::LINETRACE2:
            linetrace_->setPid();
            return fixedDistanceLineTrace(450 - LCOURSE_SHORTCUT_LENGTH,100,LineTraceEdge::LEFT);



        case Phase::CURVE3:
            linetrace_->setPid(0.006,0,0.4);
            linetrace_->setMaxPwm(100);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(900,0);


        case Phase::LINETRACE3:
            linetrace_->setPid();
            return fixedDistanceLineTrace(650,100,LineTraceEdge::LEFT);


        case Phase::CURVE4:
            linetrace_->setPid(0.006,0,0.4);
            linetrace_->setMaxPwm(100);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(750,10);


        case Phase::CURVE5:
            linetrace_->setPid(0.006,0,0.4);
            linetrace_->setMaxPwm(100);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            return fixedDistanceCurveLineTrace(950,-100);


        case Phase::LINETRACE4:
            linetrace_->setPid();
            return fixedDistanceLineTrace(1700,100,LineTraceEdge::LEFT);


        case Phase::CHANGEEDGE:
            return linetrace_->changeEdge();

        case Phase::CHANGEEDGE_R_L:
            startDistanceMeasurement(100);
            curveRunning_->run(60,48);
            return distanceMeasurement_->getResult();

        case Phase::CHANGEEDGE_L_R:
            startDistanceMeasurement(100);
            curveRunning_->run(48,60);
            return distanceMeasurement_->getResult();

        //ライン無視走行
        case Phase::LINE_IGNORE:
            linetrace_->setPid(0,0,0);
            linetrace_->setMaxPwm(60);
            return fixedDistanceCurveLineTrace(200,320);

        default: return false;
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
