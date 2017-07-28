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
            case Phase::ADJUST1:
                //ブロック並べ終点から位置調整用
                return true;
            
            case Phase::PIVOT_TURN1:
                return pivotTurn_->turn2(-90);

            case Phase::APPROACH:
                distanceMeasurement_->start(100);
                straightRunning_->run(40);
                return lineDetection_->getResult() && distanceMeasurement_->getResult();

            case Phase::PIVOT_TURN2:
                return pivotTurn_->turn(75);

            case Phase::LINETRACE:
                distanceMeasurement_->start(380); //要調整
                lineTrace_->run(40,LineTraceEdge::LEFT);
                return distanceMeasurement_->getResult();

            case Phase::PIVOT_TURN3:
                return pivotTurn_->turn2(90);

            case Phase::ENTRY:
                distanceMeasurement_->start(50); //要調整
                straightRunning_->run(30);
                return distanceMeasurement_->getResult();

            case Phase::PIVOT_TURN4:
                return pivotTurn_->turn2(-90);

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