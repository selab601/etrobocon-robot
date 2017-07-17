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
        switch(phase_){

        //色検知するまでライントレース
        case Phase::START_LINE_TRACE:
            lineTrace_->setPid();
            lineTrace_->setTarget();
            lineTrace_->run(20,LineTraceEdge::RIGHT);
            if(colorDetection_->isFourColors()){
                bodyAngleMeasurement_->setBaseAngle();//検知した時の角度保存
                phase_ = Phase::CURVE;
            }
            break;

        //カーブ走行（信地旋回）
        case Phase::CURVE:
            curveRunning_->run(0,20);
            if(bodyAngleMeasurement_->getResult() >= 90){
                phase_ = Phase::STRAIGHT;
            }
            break;

        //直進走行
        case Phase::STRAIGHT:
            if(digree >= 90){//90度より大きなカーブのときのみ
                distanceMeasurement_->start(20);
                straightRunning_->run(20);
            }else{
                phase_ = Phase::END_LINE_TRACE;
            }
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::END_LINE_TRACE;
            }
            break;

        case Phase::END_LINE_TRACE:
            distanceMeasurement_->start(100);
            lineTrace_->run(20,LineTraceEdge::RIGHT);
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::START_LINE_TRACE;
                return true;
            }
            break;
        }
        return false;
    }


}
