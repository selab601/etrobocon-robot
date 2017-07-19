#include "ParkingL.h"

using namespace detection;
using namespace device;
using namespace drive;
using namespace measurement;

namespace strategy{
    ParkingL::ParkingL(){
        arm_                   = Arm::getInstance();
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
            case Phase::ADJUST:
                //ブロック並べ終点から,後退して位置調整
                distanceMeasurement_->start(185);
                straightRunning_->run(-25);
                return distanceMeasurement_->getResult();
            
            case Phase::PIVOT_TURN1:
                return pivotTurn_->turn(-90);

            case Phase::APPROACH:
                straightRunning_->run(40);
                return lineDetection_->getResult();

            case Phase::PIVOT_TURN2:
                return pivotTurn_->turn(90);

            case Phase::LINETRACE:
                distanceMeasurement_->start(500); //you
                lineTrace_->run(40,LineTraceEdge::LEFT);
                return distanceMeasurement_->getResult();

            case Phase::PIVOT_TURN3:
                return pivotTurn_->turn(85);

            case Phase::ENTRY:
                distanceMeasurement_->start(190); //要調整
                straightRunning_->run(30);
                return arm_->up() && distanceMeasurement_->getResult();

            case Phase::PIVOT_TURN4:
                return pivotTurn_->turn(-90);

            case Phase::FINISH:
                straightRunning_->run(0);
                return arm_->normal();

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