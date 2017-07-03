#include "Efforts.h"

using namespace drive;
using namespace detection;

namespace drive{
    Efforts::Efforts(){
        lineTrace_            = LineTrace::getInstance();
        pivotTurn_            = new PivotTurn();
        curveRunning_         = new CurveRunning();
        blockColorGetter_     = new BlockColorGetter();
        pushingOutRunning_    = new PushingOutRunning();
        forcingOutRunning_    = new ForcingOutRunning();
        lineDetection_        = new LineDetection();
        colorDetection_       = new ColorDetection();
        rightAngledDetection_ = new RightAngledDetection();

        result_  = new colorset_t;

        procedureNumber_    = 0;
        isSuccess_          = false;
        positionNumber_     = 0;
        startEdge_          = LineTraceEdge::RIGHT;
        endEdge_            = LineTraceEdge::RIGHT;
        isRightCurve_       = false;
        isRightForcingOut_  = false;
        turnAngle_          = 0;
    }

    bool Efforts::run(int positionNumber){
        positionNumber_ = positionNumber;
        if(!isSuccess_){
            //手順を1つずつ実行する
            if(executePhase(phaseProcedure_[procedureNumber_] ) ){
                procedureNumber_++;
            }
        }
        //最後の手順まで
        if(procedureNumber_ == phaseProcedure_.size()){
            isSuccess_ = true;
            return true;
        }
        return isSuccess_;
    }

    bool Efforts::executePhase(Phase phase){
        switch(phase){

        case Phase::INIT:

            switch(positionNumber_){
            case 1:
                isRightForcingOut_  = true;
                isRightCurve_       = true;
                turnAngle_          = 200;
                startEdge_          = LineTraceEdge::RIGHT;
                endEdge_            = LineTraceEdge::LEFT;
                return true;

            case 2:
                isRightForcingOut_  = false;
                isRightCurve_       = false;
                turnAngle_          = -200;
                startEdge_          = LineTraceEdge::LEFT;
                endEdge_            = LineTraceEdge::RIGHT;
                return true;

            case 3:
                isRightForcingOut_  = false;
                isRightCurve_       = true;
                turnAngle_          = -150;
                startEdge_          = LineTraceEdge::RIGHT;
                endEdge_            = LineTraceEdge::LEFT;
                return true;

            case 4:
                isRightForcingOut_  = true;
                isRightCurve_       = false;
                turnAngle_          = 150;
                startEdge_          = LineTraceEdge::LEFT;
                endEdge_            = LineTraceEdge::RIGHT;
                return true;

            default: return false;
            }

        case Phase::LINETRACE_TO_DAIZA:
            // lineTraceReset();
            lineTrace_->setPid();
            lineTrace_->run(15,startEdge_,0.4);
            return colorDetection_->isFourColors();

        case Phase::BLOCK_COLOR_GET:
            return blockColorGetter_->isExecuted(result_);

        case Phase::KIMARITE:
            if(result_->tableColor == result_->blockColor){
                return forcingOutRunning_->run(20,isRightForcingOut_);
            }else{
                pushingOutRunning_->run(20,100);
            }

        case Phase::PIVORT_TURN:
            return pivotTurn_->turn(turnAngle_,40);

        case Phase::TURN_TO_LINE:
            if(isRightCurve_){
                curveRunning_->run(-10,10);
            }else{
                curveRunning_->run(10,-10);
            }
            return lineDetection_->getResult();

        case Phase::LINETRACE_RIGHT_ANGLED:
            lineTrace_->run(15,endEdge_,0.4);
            return rightAngledDetection_->getResult();
        }
        return false;
    }
}
