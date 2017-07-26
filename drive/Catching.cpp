#include "./Catching.h"

using namespace device;
using namespace measurement;
using namespace detection;
namespace drive{

    Catching::Catching(){
        lineTrace_ = LineTrace::getInstance();
        straightRunning_ = new StraightRunning();
        curveRunning_ = new CurveRunning();
        pivotTurn_ = new PivotTurn();
        colorDetection_ = new ColorDetection();
        bodyAngleMeasurement_ = new BodyAngleMeasurement();
        distanceMeasurement_ = new DistanceMeasurement();
        selfPositionEstimation_ = SelfPositionEstimation::getInstance();
    }

    bool Catching::run(int currentMm, int dstMm, int degree){
        int absDegree = abs(degree);//曲がる角度(正の値)

        startEdge_ = lineTrace_->getEdge();//直前のライントレースのエッジをもらう

        if(absDegree == 180){//180度の場合エッジによって適切な方向へ旋回するように
            if(startEdge_ == LineTraceEdge::LEFT && degree > 0){
                degree = -180;
            }else if(startEdge_ == LineTraceEdge::RIGHT && degree < 0){
                degree = 180;
            }
        }

        switch(phase_){

        //色検知するまでライントレース
        case Phase::START_LINE_TRACE:
            lineTrace_->setPid();
            lineTrace_->setTarget();
            lineTrace_->setEdge(startEdge_);//セットしないとLineTrace._edgeが更新されない
            lineTrace_->run(CATCHING_LINETRACE_PWM,startEdge_);
            if(colorDetection_->isFourColors()){
                bodyAngleMeasurement_->setBaseAngle();//検知した時の角度保存
                phase_ = Phase::STRAIGHT_LITTLE;
            }
            break;

        //タイヤの中心を円周上に
        case Phase::STRAIGHT_LITTLE:
            distanceMeasurement_->start(WHEEL_TO_COLOR_SENSOR - COLOR_DETECTION_DISTANCE);
            straightRunning_->run(10);
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::PIVOT_FIRST;
                distanceMeasurement_->reset();
            }
            break;

        //最初の旋回
        case Phase::PIVOT_FIRST:
             if(pivotTurn_->turn(degree / 2,10)){
                phase_ = Phase::STRAIGHT;
             }
             break;

        //二回目の旋回
        case Phase::PIVOT_SECOND:
            if(pivotTurn_->turn(degree / 2,10)){
                phase_ = Phase::CALC_DISTANCE;
            }
            break;




        //直進走行
        case Phase::STRAIGHT:
            distanceMeasurement_->start(cos((degree / 2) * M_PI / 180) * DAIZA_DIAMETER);
            straightRunning_->run(CATCHING_PWM);
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::PIVOT_SECOND;
                distanceMeasurement_->reset();
            }
            break;


        case Phase::CALC_DISTANCE:
            //目的ラインの半分　ー　円の半径　進む
            runningDistance_ = dstMm/2 - DAIZA_DIAMETER/2;
            if(startEdge_ == LineTraceEdge::RIGHT){//右エッジスタートの場合
                if(degree > 0){//左カーブの場合
                    runningDistance_ += 10;
                }else{//右カーブの場合
                    runningDistance_ -= 10;
                }
            }else{//左エッジスタートの場合
                if(degree > 0){//左カーブの場合
                    runningDistance_ -= 10;
                }else{//右カーブの場合
                    runningDistance_ += 10;
                }
            }
            phase_ = Phase::END_LINE_TRACE;
            break;


        //カーブ後のライントレース
        case Phase::END_LINE_TRACE:
            distanceMeasurement_->start(runningDistance_);
            endEdge_ = startEdge_;
            lineTrace_->setEdge(endEdge_);
            lineTrace_->run(CATCHING_LINETRACE_PWM,endEdge_);
            if(distanceMeasurement_->getResult()){
                distanceMeasurement_->reset();
                phase_ = Phase::START_LINE_TRACE;
                return true;
            }
            break;
        }
        return false;
    }

    bool Catching::putBlock(int lineDistance){
        static bool isDaizaDetected = false;
        if(!isDaizaDetected){
            startEdge_ = lineTrace_->getEdge();
            lineTrace_->setPid();
            lineTrace_->setEdge(startEdge_);
            lineTrace_->run(CATCHING_LINETRACE_PWM,startEdge_);
            if(colorDetection_->isFourColors()){
                isDaizaDetected = true;
            }
        }else{
            distanceMeasurement_->start(int(lineDistance / 2) - DAIZA_DIAMETER / 2 - WHEEL_TO_COLOR_SENSOR + COLOR_DETECTION_DISTANCE);//ラインの中心にタイヤの中心がくるように
            straightRunning_->run(-CATCHING_LINETRACE_PWM);
            if(distanceMeasurement_->getResult()){
                isDaizaDetected =false;//フラグを戻しておく
                distanceMeasurement_->reset();
                return true;
            }
        }
        return false;
    }


}
