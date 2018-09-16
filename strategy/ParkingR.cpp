#include "ParkingR.h"
#include "detection/ColorDetection.h"

using namespace drive;
using namespace detection;
using namespace measurement;

namespace strategy{
    ParkingR::ParkingR(){
        lineTrace_             = LineTrace::getInstance();
        straightRunning_       = new StraightRunning();
        distanceMeasurement_   = new DistanceMeasurement();
        timeMeasurement_       = new TimeMeasurement();
        rightAngledDetection_  = new RightAngledDetection();
        pivotTurn_             = new PivotTurn();
        colorSensor_           = device::ColorSensor::getInstance();

        strategySuccess_ = false;
    }

    bool ParkingR::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            if(executePhase(phaseProcedure_[procedureNumber])){
                procedureNumber++;
                distanceMeasurement_->reset();
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
            case Phase::INIT:
                lineTrace_->reset();
                lineTrace_->setPid(LineTracePid::VERY_FAST);
                return true;

            case Phase::STRAIGHT1:
                //誤検知防止のため開始後15cmは直角検知しないので，開始位置によっては調整してください
                distanceMeasurement_->start(150);
                straightRunning_->run(20);
                return distanceMeasurement_->getResult() && rightAngledDetection_->getResult();

            case Phase::ADJUST1:
                distanceMeasurement_->start(20);
                straightRunning_->run(20);
                return distanceMeasurement_->getResult();

            case Phase::TURN_LEFT:
                return pivotTurn_->circleTurn(100);

            case Phase::STRAIGHT2:
                distanceMeasurement_->start(500);
                straightRunning_->run(20);
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
            
            default:
                return false;
        }
    }
}