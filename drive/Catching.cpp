#include "./Catching.h"

using namespace device;
using namespace measurement;
using namespace detection;
namespace drive{

    Catching::Catching(){
        lineTrace_ = LineTrace::getInstance();
        straightRunning_ = new StraightRunning();
        curveRunning_ = new CurveRunning();
        colorDetection_ = new ColorDetection();
        bodyAngleMeasurement_ = new BodyAngleMeasurement();
        distanceMeasurement_ = new DistanceMeasurement();
    }

    bool Catching::run(int digree){
        static int diffDigree = abs(180 - digree);//機体から見た角度(正の値)

        switch(phase_){

        //色検知するまでライントレース
        case Phase::START_LINE_TRACE:
            static LineTraceEdge startEdge = LineTraceEdge::RIGHT;
            lineTrace_->setPid();
            lineTrace_->setTarget();
            lineTrace_->run(LINETRACE_PWM,startEdge);
            if(colorDetection_->isFourColors()){
                bodyAngleMeasurement_->setBaseAngle();//検知した時の角度保存
                phase_ = Phase::STRAIGHT;
            }
            break;



        //直進走行
        case Phase::STRAIGHT:
            if(diffDigree < 90){//90<digree<270
                distanceMeasurement_->start(50);
                straightRunning_->run(CATCHING_PWM);
            }else{//スキップ
                phase_ = Phase::CURVE;
            }
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::CURVE;
            }
            break;

        //カーブ走行（信地旋回）
        case Phase::CURVE:
            if(digree == 180){//このフェーズをスキップ
                phase_ = Phase::END_LINE_TRACE;
            }else if(digree < 180){
                curveRunning_->run(0,CATCHING_PWM);//右に新地旋回
            }else{
                curveRunning_->run(CATCHING_PWM,0);//左に信地旋回
            }
            if(abs(bodyAngleMeasurement_->getResult()) >= diffDigree){
                phase_ = Phase::END_LINE_TRACE;
            }
            break;

        //カーブ後のライントレース
        case Phase::END_LINE_TRACE:
            static LineTraceEdge endEdge;
            if(diffDigree <= 60){
                endEdge = startEdge;
            }else if(diffDigree == 90){
                endEdge = LineTraceEdge::RIGHT;
            }else{
                endEdge = LineTraceEdge::LEFT;
            }
            distanceMeasurement_->start(100);
            lineTrace_->run(LINETRACE_PWM,endEdge);//エッジ要調整
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::START_LINE_TRACE;
                return true;
            }
            break;
        }
        return false;
    }


}
