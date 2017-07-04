#include "ETSumoNeo.h"

using namespace drive;
using namespace detection;
using namespace measurement;

namespace strategy{
    ETSumoNeo::ETSumoNeo(){
        linetrace_              = LineTrace::getInstance();
        straightRunning_        = new StraightRunning();
        climbingRunning_        = new ClimbingRunning();
        pivotTurn_              = new PivotTurn();
        curveRunning_           = new CurveRunning();
        lineDetection_          = new LineDetection();
        objectDetection_        = new ObjectDetection();
        rightAngledDetection_   = new RightAngledDetection();
        timeMeasurement_        = new TimeMeasurement();
        distanceMeasurement_    = new DistanceMeasurement();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();

        hasExecutedPhase_       = false;
        strategySuccess_        = false;
        isLineTraceReset_       = false;
    }

    bool ETSumoNeo::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executeStrategy(strategyProcedure_[procedureNumber])){
                lineTraceReset();
                procedureNumber++;
                hasExecutedPhase_ = false;//フラグを戻しておく
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        if(procedureNumber == strategyProcedure_.size()){//最後まで終わったら
            strategySuccess_ = true;
            return true;
        }
        return strategySuccess_;
    }

    //戦略手順を実行する
    bool ETSumoNeo::executeStrategy(StrategyPhase strategyPhase){
        switch(strategyPhase){

        //車体角度保存
        case StrategyPhase::INIT:
            bodyAngleMeasurement_->setBaseAngle();//始めの角度をセット
            return true;

        //土俵を向くまでライントレース
        case StrategyPhase::LINE_TRACE:
            startDistanceMeasurement(900);
            linetrace_->setPid(0.0144,0.0,0.72);
            linetrace_->run(40,LineTraceEdge::RIGHT);
            //距離検知or車体角度が土俵を向いたらtrue
            return distanceMeasurement_->getResult() || bodyAngleMeasurement_->getResult() >= 180;

        //新幹線を検知するまで停止
        case StrategyPhase::STOP:
            straightRunning_->run(0);
            return objectDetection_->getResult();


        //通り過ぎてから1秒間待つ
        case StrategyPhase::WAIT_1_SEC:
            startTimeMeasurement(1000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //登壇後の動作を安定させるため少し旋回
        case StrategyPhase::TURN_LITTLE:
            return pivotTurn_->turn(climbBeforeLittleAngle_);

        //登壇走行
        case StrategyPhase::CLIMB:
            return climbingRunning_->run(50,600);

        //横を向くまで旋回
        case StrategyPhase::TURN_TO_SIDE:
            return pivotTurn_->turn(climbAfterSideFaceAngle_);

        //ラインまでバック
        case StrategyPhase::BACK_TO_LINE:
            straightRunning_->run(-15);
            return lineDetection_->getResult();

        //4cm進む
        case StrategyPhase::STRAIGHT_4_CM:
            startDistanceMeasurement(45);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //下を向くように旋回
        case StrategyPhase::TURN_TO_DOWN:
            return turn(isRightRotation_,10);

        //検知した後すこし待つ
        case StrategyPhase::WAIT_2_SEC:
            startTimeMeasurement(2000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //降壇
        case StrategyPhase::GET_OF:
            startDistanceMeasurement(450);
            straightRunning_->run(50);
            return distanceMeasurement_->getResult();

        //ライン検知
        case StrategyPhase::LINE_DETECTION:
            curveRunning_->run(20,10);
            return lineDetection_->getResult();

        //復帰後のライントレーストレース
        case StrategyPhase::LINE_RETURN:
            startDistanceMeasurement(1350);
            lineTraceReset();
            linetrace_->setEdge(LineTraceEdge::RIGHT);
            linetrace_->setMaxPwm(60);
            linetrace_->setPid(0.006,0,0.6);
            linetrace_->setTarget(0.5);
            linetrace_->runCurve(-380);
            return distanceMeasurement_->getResult();

        //一人目の力士を押し出す
        case SumoPhase::FIRST_EXTRUSION:
            return extrusion(firstWrestlerColor_);

        //二人目の力士を押し出す
        case SumoPhase::SECOND_EXTRUSION:
            return extrusion(secondWrestlerColor_);

        //三人目の力士を押し出す
        case SumoPhase::THIRD_EXTRUSION:
            return extrusion(thirdWrestlerColor_);

        //直角をまたぐ走行
        case SumoPhase::ACROSS_LINE:
            startDistanceMeasurement(50);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //一回目の旋回
        case SumoPhase::FIRST_TURN:
            return pivotTurn_->turn(firstTurnAngle_,30);

        //二回目の旋回
        case SumoPhase::SECOND_TURN:
            return pivotTurn_->turn(secondTurnAngle_,30);

        //三回目の旋回
        case SumoPhase::THIRD_TURN:
            return pivotTurn_->turn(thirdTurnAngle_);

        //旋回すると尻尾が新幹線にぶつかるのでカーブをして上を向く
        case SumoPhase::CURVE_TOP:
            return curve(isRightCurve_,30);

        //上段までライントレース
        case SumoPhase::UPPER_STAGE:
            static bool isTimeDetected = false;
            startTimeMeasurement(800);
            linetrace_->run(20,upperStageEdge_,0.4);
            if(timeMeasurement_->getResult()){
                isTimeDetected = true;
            }
            return isTimeDetected && rightAngledDetection_->getResult(4.0);

        //下段までライントレース
        case SumoPhase::DOWN_STAGE:
            static bool isTimeDetected2 = false;
            startTimeMeasurement(400);
            linetrace_->run(20,downStageEdge_,0.4);
            if(timeMeasurement_->getResult()){
                isTimeDetected2 = true;
            }
            return isTimeDetected2 && rightAngledDetection_->getResult(4.0);

        //3cm直進
        case SumoPhase::STRAIGHT_3_CM:
            startDistanceMeasurement(20);
            straightRunning_->run(20);
            return distanceMeasurement_->getResult();


        default: return false;
        }
        return false;
    }


    //ラインまで旋回
    bool ETSumoNeo::turn(bool isRight, int speed){
        if(isRight){
            curveRunning_->run(-speed,speed);
        }else{
            curveRunning_->run(speed,-speed);
        }
        return lineDetection_->getResult();
    }

    //上段に向けてカーブする
    bool ETSumoNeo::curve(bool isRight, int speed){
        if(isRight){
            curveRunning_->run(0,speed);
            return bodyAngleMeasurement_->getResult() <= -90;
        }else{
            curveRunning_->run(speed,0);
            return bodyAngleMeasurement_->getResult() >= 90;
        }
    }




    //時間検知をまとめたもの
    void ETSumoNeo::startTimeMeasurement(int time,bool flagChange){
        if(!hasExecutedPhase_){
            timeMeasurement_->setBaseTime();
            timeMeasurement_->setTargetTime(time);
            if(flagChange){
                hasExecutedPhase_ = true;
            }
        }
    }

    void ETSumoNeo::lineTraceReset(){
        if(!isLineTraceReset_){
            linetrace_->reset();
            isLineTraceReset_ = true;
        }
    }

}
