#include "LCourseStandard.h"

using namespace drive;
using namespace measurement;

namespace strategy{
    LCourseStandard::LCourseStandard(){
        linetrace_ = LineTrace::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
        strategySuccess_ = false;
        hasExecutedPhase_ = false;
    }

    bool LCourseStandard::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            if(executePhase(phaseProcedure_[procedureNumber])){
                procedureNumber++;
                hasExecutedPhase_ = false;
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
            startDistanceMeasurement(2500);
            linetrace_->setPid(0.003,0.00000033333333,0.3);
            linetrace_->run(100,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case Phase::CURVE1:
            linetrace_->setPid(0.0144,0.0,0.72);
            return fixedDistanceLineTrace(700,40,LineTraceEdge::RIGHT);

        case Phase::LINETRACE2:
            linetrace_->setPid(0.003,0.00000033333333,0.3);
            return fixedDistanceLineTrace(1400,100,LineTraceEdge::RIGHT);

        case Phase::CURVE2:
            linetrace_->setPid(0.0144,0.0,0.72);
            return fixedDistanceLineTrace(700,40,LineTraceEdge::RIGHT);

        case Phase::LINETRACE3:
            linetrace_->setPid(0.003,0.00000033333333,0.3);
            return fixedDistanceLineTrace(300,40,LineTraceEdge::RIGHT);

        case Phase::CHANGEEDGE:
            return linetrace_->changeEdge();

        case Phase::LINETRACE4:
            linetrace_->setPid(0.003,0.00000033333333,0.3);
            return fixedDistanceLineTrace(300,40,LineTraceEdge::LEFT);

        case Phase::CURVE3:
            linetrace_->setPid(0.0144,0.0,0.72);
            return fixedDistanceLineTrace(1000,40,LineTraceEdge::LEFT);

        case Phase::LINETRACE5:
            linetrace_->setPid(0.003,0.00000033333333,0.3);
            return fixedDistanceLineTrace(200,50,LineTraceEdge::LEFT);

        case Phase::LINETRACE6:
            linetrace_->setPid(0.003,0.00000033333333,0.3);
            return fixedDistanceLineTrace(600,40,LineTraceEdge::RIGHT);

        default: return false;
        }
    }

    void LCourseStandard::startDistanceMeasurement(int distance){
        if(!hasExecutedPhase_){
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            hasExecutedPhase_ = true;
        }
    }

    bool LCourseStandard::fixedDistanceLineTrace(int distance,int speed,LineTraceEdge edge){
        startDistanceMeasurement(distance);
        linetrace_->run(speed,edge);
        return distanceMeasurement_->getResult();
    }

};
