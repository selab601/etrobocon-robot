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
        efforts_                = new Efforts();
        lineDetection_          = new LineDetection();
        objectDetection_        = new ObjectDetection();
        rightAngledDetection_   = new RightAngledDetection();
        timeMeasurement_        = new TimeMeasurement();
        distanceMeasurement_    = new DistanceMeasurement();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();
        train_                  = Train::getInstance();

        hasExecutedPhase_       = false;
        strategySuccess_        = false;
        isLineTraceReset_       = false;
    }

    bool ETSumoNeo::capture(){
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executeStrategy(strategyProcedure_[procedureNumber_])){
                lineTraceReset();
                distanceMeasurement_->reset();
                efforts_->reset();
                procedureNumber_++;
                hasExecutedPhase_ = false;//フラグを戻しておく
                isLineTraceReset_ = false;
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        if(procedureNumber_ == strategyProcedure_.size()){//最後まで終わったら
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
            distanceMeasurement_->start(1170);
            linetrace_->setPid(LineTracePid::MID);
            linetrace_->setEdge(LineTraceEdge::RIGHT);
            linetrace_->runCurve(400);
            //距離検知or車体角度が土俵を向いたらtrue
            return distanceMeasurement_->getResult() || bodyAngleMeasurement_->getResult() >= 180;

        //すこしライントレース
        case StrategyPhase::LINE_TRACE_LITTLE:
            distanceMeasurement_->start(100);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(30,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        // 入り口で新幹線を検知するまで停止
        case StrategyPhase::STOP_ENTRY:
            straightRunning_->run(0);
            // 新幹線が入り口にいないと分かるとき(時間で)
            if (!train_->atEntrance()){
                ev3_speaker_play_tone(300, 4);
                return true; // TODO: 入り口でも見なくて大丈夫か実験
            }
            else{
                ev3_speaker_play_tone(900, 4);
            }
            if (objectDetection_->getResult()){
                train_->setEntrance();  // 入り口で新幹線を見たことを知らせる
                strategyProcedure_.insert( // 通過するのを待つ
                        strategyProcedure_.begin() + procedureNumber_ + 1,
                        StrategyPhase::WAIT_1_SEC);
                ev3_speaker_play_tone(700, 2000);
                return true;
            }
            return false;

        // 中央で新幹線がいなくなるまで停止
        case StrategyPhase::STOP_CENTER:
            straightRunning_->run(0);
            // 新幹線がいないと分かるとき(時間で)
            if (!train_->atCenter()){
                return true;
            }
            else{
                ev3_speaker_play_tone(900, 4);
            }
            if (objectDetection_->getResult()){
                train_->setCenter();  // 中央で新幹線を見たことを知らせる
                strategyProcedure_.insert( // 通過するのを待つ
                        strategyProcedure_.begin() + procedureNumber_ + 1,
                        StrategyPhase::WAIT_1_SEC);
                ev3_speaker_play_tone(700, 2000);
                return true;
            }
            return false;

        // 出口で新幹線がいなくなるまで停止
        case StrategyPhase::STOP_EXIT:
            straightRunning_->run(0);
            // 新幹線がいないと分かるとき(時間で)
            if (!train_->atExit()){
                return true;
            }
            else{
                ev3_speaker_play_tone(900, 4);
            }
            return false;

        //通り過ぎてから1秒間待つ
        case StrategyPhase::WAIT_1_SEC:
            startTimeMeasurement(1000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //登壇後の動作を安定させるため少し旋回
        case StrategyPhase::TURN_LITTLE:
            return pivotTurn_->turn(3);

        //登壇走行
        case StrategyPhase::CLIMB:
            return climbingRunning_->run(50, 400);

        case StrategyPhase::LINE_RETURN:
            lineTraceReset();
            distanceMeasurement_->start(160);
            linetrace_->setTarget(0.4);
            linetrace_->setMaxPwm(30);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            linetrace_->setPid(LineTracePid::RETURN);
            linetrace_->run();
            return distanceMeasurement_->getResult();

        case StrategyPhase::TO_RIGHT_ANGLE:
            lineTraceReset();
            linetrace_->setTarget(0.5);
            linetrace_->setMaxPwm(20);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run();
            return rightAngledDetection_->getResult(4.0);


        case StrategyPhase::TO_CENTER:
            polar_.back(true);
            polar_.setMaxPwm(30);
            polar_.centerPivot(true);
            return polar_.runToXY(-95, 0);

        case StrategyPhase::TO_BLOCK1:
            polar_.back(false);
            return polar_.runTo(175, 557 - bodyAngleMeasurement_->getRelative10());
        case StrategyPhase::BACK:
            polar_.back(true);
            return polar_.runTo(175, -1800 +557 - bodyAngleMeasurement_->getRelative10());
        case StrategyPhase::TO_BLOCK2:
            polar_.back(false);
            return polar_.runTo(175, 1243 - bodyAngleMeasurement_->getRelative10());
        case StrategyPhase::BACK2:
            polar_.back(true);
            return polar_.runTo(175, -1800 +1243 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK3:
            polar_.back(false);
            return polar_.runTo(175, 2357 - bodyAngleMeasurement_->getRelative10());
        case StrategyPhase::BACK3:
            polar_.back(true);
            return polar_.runTo(175, -1800 +2357 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK4:
            polar_.back(false);
            return polar_.runTo(175, 3043 - bodyAngleMeasurement_->getRelative10());
        case StrategyPhase::BACK4:
            polar_.back(true);
            return polar_.runTo(175, -1800 +3043 - bodyAngleMeasurement_->getRelative10());

        //横を向くまで旋回
        case StrategyPhase::TURN_TO_SIDE:
            return pivotTurn_->turn(85);

        //ラインまでバック
        case StrategyPhase::BACK_TO_LINE:
            straightRunning_->run(-15);
            return lineDetection_->getResult();

        //4cm進む
        case StrategyPhase::STRAIGHT_4_CM:
            distanceMeasurement_->start(40);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //下を向くように旋回
        case StrategyPhase::TURN_TO_DOWN:
            return turn(false,10);

        //3cm直進
        case StrategyPhase::STRAIGHT_2_CM:
            distanceMeasurement_->start(20);
            straightRunning_->run(20);
            return distanceMeasurement_->getResult();

        //右に90度旋回
        case StrategyPhase::TURN_RIGHT_90:
            return pivotTurn_->turn(-90);

        //左に90度旋回
        case StrategyPhase::TURN_LEFT_90:
            return pivotTurn_->turn(90);

        //一回目の取組
        case StrategyPhase::FIRST_EFFORTS:
            lineTraceReset();
            return efforts_->run(1);

        //二回目の取組
        case StrategyPhase::SECOND_EFFORTS:
            lineTraceReset();
            return efforts_->run(2);

        //三回目の取組
        case StrategyPhase::THIRD_EFFORTS:
            lineTraceReset();
            return efforts_->run(4);

        //四回目の取組
        case StrategyPhase::FOURTH_EFFORTS:
            lineTraceReset();
            return efforts_->run(3);

        //直角をまたぐ走行
        case StrategyPhase::ACROSS_LINE:
            distanceMeasurement_->start(50);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //旋回すると尻尾が新幹線にぶつかるのでカーブをして上を向く
        case StrategyPhase::CURVE_TOP:
            return curve(true,30);

        //上段までライントレース
        case StrategyPhase::UPPER_STAGE:
            lineTraceReset();
            distanceMeasurement_->start(50);
            linetrace_->run(20,LineTraceEdge::LEFT,0.6);
            return distanceMeasurement_->getResult() && rightAngledDetection_->getResult(4.0);

        //下段までライントレース
        case StrategyPhase::DOWN_STAGE:
            lineTraceReset();
            distanceMeasurement_->start(40);
            linetrace_->run(20,LineTraceEdge::RIGHT,0.6);
            return distanceMeasurement_->getResult() && rightAngledDetection_->getResult(4.0);

        //検知した後すこし待つ
        case StrategyPhase::WAIT_2_SEC:
            startTimeMeasurement(2000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        case StrategyPhase::NEXT_STAGE:
            distanceMeasurement_->start(500);
            straightRunning_->run(60);
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
