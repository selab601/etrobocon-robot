#include "Efforts.h"

using namespace drive;
using namespace detection;
using namespace measurement;

namespace drive{
    Efforts::Efforts(){
        lineTrace_            = LineTrace::getInstance();
        pivotTurn_            = new PivotTurn();
        curveRunning_         = new CurveRunning();
        blockColorGetter_     = BlockColorGetter();
        pushingOutRunning_    = new PushingOutRunning();
        forcingOutRunning_    = new ForcingOutRunning();
        straightRunning_      = new StraightRunning();
        lineDetection_        = new LineDetection();
        colorDetection_       = new ColorDetection();
        rightAngledDetection_ = new RightAngledDetection();
        distanceMeasurement_  = new DistanceMeasurement();

        result_  = new colorset_t;

        procedureNumber_    = 0;
        isSuccess_          = false;
        positionNumber_     = 0;
        startEdge_          = LineTraceEdge::RIGHT;
        endEdge_            = LineTraceEdge::RIGHT;
        isRightCurve_       = false;
        isRightForcingOut_  = false;
        turnAngle_          = 0;
        turnAngleLittle_    = 0;
    }

    bool Efforts::run(int positionNumber){
        positionNumber_ = positionNumber;
        if(!isSuccess_){
            //手順を1つずつ実行する
            if(executePhase(phaseProcedure_[procedureNumber_] ) ){
                procedureNumber_++;
                distanceMeasurement_->reset();
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
        //取組位置に応じた初期化
        case Phase::INIT:

            switch(positionNumber_){
            //登壇前の走行体の位置からみて
            //左手前:1,右手前:2,左奥:3,右奥4
            case 1:
                //寄り切り走行の向き
                isRightForcingOut_  = false;
                //旋回後のライン復帰の向き
                isRightCurve_       = true;
                //旋回角度
                turnAngle_          = 210;
                turnAngleLittle_    = 10;
                //台座までのライントレースのエッジ
                startEdge_          = LineTraceEdge::RIGHT;
                //帰るときのライントレースのエッジ
                endEdge_            = LineTraceEdge::LEFT;
                return true;

            case 2:
                isRightForcingOut_  = true;
                isRightCurve_       = false;
                turnAngle_          = -210;
                turnAngleLittle_    = 10;
                startEdge_          = LineTraceEdge::LEFT;
                endEdge_            = LineTraceEdge::RIGHT;
                return true;

            case 3:
                isRightForcingOut_  = true;
                isRightCurve_       = true;
                turnAngle_          = -150;
                startEdge_          = LineTraceEdge::RIGHT;
                endEdge_            = LineTraceEdge::LEFT;
                return true;

            case 4:
                isRightForcingOut_  = false;
                isRightCurve_       = false;
                turnAngle_          = 150;
                startEdge_          = LineTraceEdge::LEFT;
                endEdge_            = LineTraceEdge::RIGHT;
                return true;

            default: return false;
            }

        //台座に向かうライントレース
        case Phase::LINETRACE_TO_DAIZA:
            // lineTraceReset();
            lineTrace_->setPid(LineTracePid::MID);
            lineTrace_->run(15,startEdge_,0.4);
            return colorDetection_->isFourColors();

        //ブロック色取得
        case Phase::BLOCK_COLOR_GET:
            return blockColorGetter_.isExecuted(result_);

        //取組
        case Phase::KIMARITE:
            if(result_->tableColor == result_->blockColor){
                return forcingOutRunning_->run(20,isRightForcingOut_);
            }else{
                return pushingOutRunning_->run(20,100);
            }

        //バック
        case Phase::BACK:
            if(positionNumber_ == 1 || positionNumber_ == 2){
                distanceMeasurement_->start(10);
                straightRunning_->run(-30);
                return distanceMeasurement_->getResult();
            }else{
                return true;
            }
        //旋回
        case Phase::PIVORT_TURN:
            return pivotTurn_->turn(turnAngle_,40);

        //すこし旋回
        case Phase::PIVORT_TURN_LITTLE:
            if(positionNumber_ == 1 || positionNumber_ == 2){
                return pivotTurn_->turn(turnAngleLittle_,20);
            }else{
                return true;
            }

        //ライン復帰
        case Phase::TURN_TO_LINE:
            if(isRightCurve_){
                curveRunning_->run(-10,10);
            }else{
                curveRunning_->run(10,-10);
            }
            return lineDetection_->getResult();

        //帰りのライントレース
        case Phase::LINETRACE_RIGHT_ANGLED:
            distanceMeasurement_->start(30);
            lineTrace_->run(15,endEdge_,0.6);
            return distanceMeasurement_->getResult() && rightAngledDetection_->getResult();
        }
        return false;
    }

    void Efforts::reset(){
        procedureNumber_    = 0;
        isSuccess_          = false;
        positionNumber_     = 0;
        isRightForcingOut_  = false;
        isRightCurve_       = false;
        turnAngle_          = 0;
        startEdge_          = LineTraceEdge::RIGHT;
        endEdge_            = LineTraceEdge::RIGHT;
        blockColorGetter_   = BlockColorGetter();
        forcingOutRunning_->reset();
    }
}
