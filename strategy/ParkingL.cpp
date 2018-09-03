#include "ParkingL.h"

using namespace detection;
using namespace drive;
using namespace measurement;

namespace strategy{
    ParkingL::ParkingL(){
        lineTrace_             = LineTrace::getInstance();
        pivotTurn_             = new PivotTurn();
        straightRunning_       = new StraightRunning();
        distanceMeasurement_   = new DistanceMeasurement();
        timeMeasurement_       = new TimeMeasurement();
        lineDetection_         = new LineDetection();

        strategySuccess_ = false;
    }

    bool ParkingL::capture(){
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

    bool ParkingL::executePhase(Phase phase){
        switch(phase){
/*            case Phase::LINETRACE:
                distanceMeasurement_->start(500); //要調整
                lineTrace_->run(40,LineTraceEdge::LEFT);
                return distanceMeasurement_->getResult();
*/
            case Phase::LINETRACE:
                distanceMeasurement_->start(100); //要調整 //調整しました（山川）
                lineTrace_->run(40,LineTraceEdge::LEFT);
                return distanceMeasurement_->getResult();

            case Phase::TURN_LEFT_60:
                return pivotTurn_->circleTurn(60);
            
            case Phase::STRAIGHT_36:
                distanceMeasurement_->start(380);//調整しました（山川）
                straightRunning_->run(40);
                return distanceMeasurement_->getResult();
            
            case Phase::TURN_RIGHT_90:
                return pivotTurn_->circleTurn(-90);

            case Phase::FINISH:
                straightRunning_->run(0);
                return false;

            case Phase::WAIT:
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
