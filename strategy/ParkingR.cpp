#include "ParkingR.h"

using namespace drive;
using namespace detection;
using namespace measurement;

namespace strategy{
    ParkingR::ParkingR(){
        lineTrace_             = LineTrace::getInstance();
        distanceMeasurement_   = new DistanceMeasurement();
        timeMeasurement_       = new TimeMeasurement();
        rightAngledDetection_  = new RightAngledDetection();
        pivotTurn_             = new PivotTurn();

        strategySuccess_ = false;
    }

    bool ParkingR::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            if(executePhase(phaseProcedure_[procedureNumber])){
                procedureNumber++;
                ev3_speaker_set_volume(6);
                ev3_speaker_play_tone(500,100);
            }
        }
        if(procedureNumber == phaseProcedure_.size()){
            strategySuccess_ = true;
        }
        return strategySuccess_;
    }

    bool ParkingR::executePhase(Phase phase){
        switch(phase){
            case Phase::LINE_TRACE1:
                //線に入射したときも直角検知するため、距離の条件追加
                distanceMeasurement_->start(100);
                lineTrace_->run(40,LineTraceEdge::RIGHT);
                return distanceMeasurement_->getResult() && rightAngledDetection_->getResult();

            case Phase::TURN_RIGHT:
                return pivotTurn_->turn(-90);

            case Phase::LINE_TRACE2:
                distanceMeasurement_->start(400);
                lineTrace_->run(40,LineTraceEdge::RIGHT);
                return distanceMeasurement_->getResult();

            case Phase::WAIT:
                lineTrace_->run(0,LineTraceEdge::RIGHT);
                /*
                //x秒経つまでfalse,経ったらtrueでphase終了,にしたい
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(5000);
                return timeMeasurement_->getResult();
                */
                return false;
                //現状無限にfalse返して停止してます,改良するかも
        }
    }
}