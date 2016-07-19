#include "ETSumoNeo.h"

using namespace drive;
using namespace detection;
using namespace measurement;

namespace strategy{

    ETSumoNeo::ETSumoNeo(){
        linetrace_ = LineTrace::getInstance();
        straightRunning_ = new StraightRunning();
        climbingRunning_ = new ClimbingRunning();
        extrusionRunning_ = new ExtrusionRunning();
        pivotTurn_ = new PivotTurn();
        lineDetection_ = new LineDetection();
        colorDetection_ = new ColorDetection();
        objectDetection_ = new ObjectDetection();
        distanceMeasurement_ = new DistanceMeasurement();
        statas_ = Statas::HOSHITORI;
        //sumoStatas = SmoStatas:: ;
    }

    bool ETSumoNeo::capture(){
        colorid_t nowColor = COLOR_NONE;
        switch(statas_){

        //星取取得
        case Statas::HOSHITORI:
            nowColor = colorDetection_->getResult();
            if(nowColor == COLOR_BLUE || nowColor == COLOR_RED ||
                nowColor == COLOR_GREEN || nowColor == COLOR_YELLOW){
                hoshitori_ = nowColor;//星取保存
                statas_ = Statas::TURN;
            }
            break;

        //左に90度旋回
        case Statas::TURN:
            if(pivotTurn_->turn(90)){
                distanceMeasurement_->setTargetDistance(200);
                distanceMeasurement_->startMeasurement();
                statas_ = Statas::STRAIGHT;
            }
            break;

        //ラインに近づくように直進走行
        case Statas::STRAIGHT:
            straightRunning_->run(30);
            if(distanceMeasurement_->getResult()){
                distanceMeasurement_->setTargetDistance(1000);
                distanceMeasurement_->startMeasurement();
                statas_ = Statas::LINE_TRACE;
            }
            break;

        //土俵手前までライントレース
        case Statas::LINE_TRACE:
            linetrace_->run(30);//右エッジ
            if(distanceMeasurement_->getResult()){
                statas_ = Statas::STOP;
            }
            break;

        //新幹線が通り過ぎるまで停止
        case Statas::STOP:
            straightRunning_->run(0);
            if(objectDetection_->getResult()){
                statas_ = Statas::CLIMB;
            }
            break;

        //登壇走行
        case Statas::CLIMB:
            if(climbingRunning_->run(50,400)){
                statas_ = Statas::SUMO;
            }
            break;

        // //相撲
         case Statas::SUMO:
        //     if(performSumo()){
        //         distanceMeasurement_->setTargetDistance(300);
        //         distanceMeasurement_->startMeasurement();
        //         statas_ = Statas::GET_OF;
        //     }

        // //降壇
         case Statas::GET_OF:
        //     if(distanceMeasurement_->getResult()){
        //         return true;
        //     }
        //     break;
         return false;

        }
    return false;
    }

    bool ETSumoNeo::performSumo(){
        // switch(sumoStatas){
        // }
        return true;
    }

}
