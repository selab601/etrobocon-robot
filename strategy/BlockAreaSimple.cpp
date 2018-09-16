#include "BlockAreaSimple.h"

using namespace drive;
using namespace detection;
using namespace device;
using namespace measurement;

namespace strategy{
    BlockAreaSimple::BlockAreaSimple(){
        lineTrace_              = LineTrace::getInstance();
        lineDetection_          = new LineDetection();
        rightAngledDetection_   = new RightAngledDetection();
        distanceMeasurement_    = new DistanceMeasurement();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();
        straightRunning_        = new StraightRunning();
        pivotTurn_              = new PivotTurn();

        strategySuccess_        = false;
        isLineTraceReset_       = false;
    }

    bool BlockAreaSimple::capture(){
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executeStrategy(strategyProcedure_[procedureNumber_])){
                lineTraceReset();
                polar_.reset();
                distanceMeasurement_->reset();
                procedureNumber_++; //フラグを戻しておく
                hasExecutedPhase_ = false;
                isLineTraceReset_ = false;
                ev3_speaker_play_tone( 500, 100);//音を出す
            }
        }
        if(procedureNumber_ == strategyProcedure_.size()){//最後まで終わったら
            strategySuccess_ = true;
            return true;
        }
        return strategySuccess_;
    }

    //戦略手順を実行する
    bool BlockAreaSimple::executeStrategy(StrategyPhase strategyPhase){

        //baseAngleに対する車体の相対角度を保存
        static int diffAngle;

        switch(strategyPhase){

        //基底となるbaseAngleを保存
        case StrategyPhase::INIT:
            bodyAngleMeasurement_->setBaseAngle();
            polar_.setMaxPwm(20);
            lineTrace_->setPid(LineTracePid::MID);
            //diffAngle = bodyAngleMeasurement_->getRelative10();
            return true;

        case StrategyPhase::ENTRY:
            distanceMeasurement_->start(100);
            lineTrace_->run(10,LineTraceEdge::LEFT);
            if(distanceMeasurement_->getResult() && rightAngledDetection_->getResult(4.0)){
                ev3_speaker_play_tone(100, 3000);
                return true;
            }
            //return distanceMeasurement_->getResult();
            return false;

        //青台座まで極座標で突進版
        case StrategyPhase::TOSSHIN:
            return polar_.runTo(900, 0);

        //突進ではなくライントレース版
        case StrategyPhase::TO_BLUE:
            distanceMeasurement_->start(900);
            lineTrace_->run(20,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        //置いてバック
        case StrategyPhase::BACK:
            distanceMeasurement_->start(150);
            straightRunning_->run(-10);
            return distanceMeasurement_->getResult();

        //台座を避ける
        case StrategyPhase::AVOID1:
            //return polar_.runTo(300, 450, 450);
            //return pivotTurn_->turn(45 - bodyAngleMeasurement_->getResult(),10);
            return pivotTurn_->circleTurn(45, 20);

        case StrategyPhase::AVOID2:
            //polar_.runTo(500, -900, -900);
            straightRunning_->run(20);
            return rightAngledDetection_->getResult();

        case StrategyPhase::AVOID3:
            //return polar_.bodyTurn(450, 10);
            //return pivotTurn_->turn(-45 - bodyAngleMeasurement_->getResult(),10);
            return pivotTurn_->circleTurn(-90, 20);

        case StrategyPhase::AVOID4:
            straightRunning_->run(20);
            return rightAngledDetection_->getResult();

        case StrategyPhase::AVOID5:
            //return pivotTurn_->turn(0 - bodyAngleMeasurement_->getResult(),10);
            return pivotTurn_->circleTurn(45, 20);

        case StrategyPhase::LINE_TRACE:
            distanceMeasurement_->start(200);
            lineTrace_->run(20,LineTraceEdge::LEFT);
            //return rightAngledDetection_->getResult(4.0);
            return distanceMeasurement_->getResult();

        case StrategyPhase::SET_DIFFANGLE:
            diffAngle = bodyAngleMeasurement_->getRelative10();
            return true;

        case StrategyPhase::FIX_ANGLE:
            return pivotTurn_->turn(-bodyAngleMeasurement_->getResult(), 10);
            //return polar_.bodyTurn(-diffAngle, 10);

        default:
            return false;
        }
        return false;
    }

    void BlockAreaSimple::lineTraceReset(){
        if(!isLineTraceReset_){
            lineTrace_->reset();
            isLineTraceReset_ = true;
        }
    }

}
