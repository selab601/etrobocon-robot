#include "LCourseStandard.h"

using namespace drive;
using namespace measurement;

namespace strategy{
    LCourseStandard::LCourseStandard(){
        linetrace_ = LineTrace::getInstance();
        distanceMeasurement_ = new DistanceMeasurement();
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
            startDistanceMeasurement(2500);
            lineTraceReset();
            linetrace_->setPid();
            linetrace_->run(80,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case Phase::CURVE1:
            linetrace_->setPid(0.006,0,0.4);
            linetrace_->setMaxPwm(60);
            return fixedDistanceCurveLineTrace(700,-1050);

        case Phase::LINETRACE2:
            linetrace_->setPid();
            return fixedDistanceLineTrace(1250,80,LineTraceEdge::RIGHT);

        case Phase::CURVE2:
            linetrace_->setPid(0.006,0,0.4);
            linetrace_->setMaxPwm(70);
            return fixedDistanceCurveLineTrace(700,380);

        case Phase::LINETRACE3:
            //エッジ切り替え直前
            linetrace_->setPid();
            //return fixedDistanceLineTrace(300,50,LineTraceEdge::RIGHT);
            return fixedDistanceLineTrace(870,70,LineTraceEdge::RIGHT);

        case Phase::CHANGEEDGE:
            //startDistanceMeasurement(300);
            return linetrace_->changeEdge();/* || distanceMeasurement_->getResult();*/

        case Phase::LINETRACE4:
            //エッジ切り替え直後
            linetrace_->setPid();
            return fixedDistanceLineTrace(300,50,LineTraceEdge::LEFT);

        case Phase::CURVE3:
            linetrace_->setPid(0.006,0,0.4);
            linetrace_->setMaxPwm(60);
            return fixedDistanceCurveLineTrace(700,320);

        case Phase::LINETRACE5:
            //エッジ切り替え直前
            linetrace_->setPid();
            return fixedDistanceLineTrace(400,40,LineTraceEdge::LEFT);

        case Phase::LINETRACE6:
            //エッジ切り替え直後
            //エッジ切り替えに使う距離が毎回違うので早めに終了させる
            linetrace_->setPid();
            //return fixedDistanceLineTrace(250,40,LineTraceEdge::RIGHT);
            return fixedDistanceLineTrace(800,40,LineTraceEdge::RIGHT);

        case Phase::LINE_IGNORE:
            linetrace_->setPid(0,0,0);
            linetrace_->setMaxPwm(60);
            return fixedDistanceCurveLineTrace(200,320);

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
