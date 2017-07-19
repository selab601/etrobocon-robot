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
        int absDigree = abs(digree);//曲がる角度(正の値)

        startEdge_ = lineTrace_->getEdge();//直前のライントレースのエッジをもらう

        if(absDigree == 180){//180度の場合エッジによって適切な方向へ旋回するように
            if(startEdge_ == LineTraceEdge::LEFT && digree > 0){
                digree = -180;
            }else if(startEdge_ == LineTraceEdge::RIGHT && digree < 0){
                digree = 180;
            }
        }

        switch(phase_){

        //色検知するまでライントレース
        case Phase::START_LINE_TRACE:
            lineTrace_->setPid();
            lineTrace_->setTarget();
            lineTrace_->run(CATCHING_LINETRACE_PWM,startEdge_);
            if(colorDetection_->isFourColors()){
                bodyAngleMeasurement_->setBaseAngle();//検知した時の角度保存
                phase_ = Phase::STRAIGHT;
            }
            break;



        //直進走行
        case Phase::STRAIGHT:
            if(absDigree <= 90){//90度より曲がらない場合
                if(digree == 0){//0度の場合は多めに直進
                    distanceMeasurement_->start(110);
                }else if(absDigree == 90){//90度の場合は少なめに直進
                    distanceMeasurement_->start(30);
                }else{
                    distanceMeasurement_->start(40);
                }
                straightRunning_->run(CATCHING_PWM);
            }else{//スキップ
                phase_ = Phase::CURVE;
            }
            if(distanceMeasurement_->getResult()){
                distanceMeasurement_->reset();
                phase_ = Phase::CURVE;
            }
            break;

        //カーブ走行（信地旋回）
        case Phase::CURVE:
            if(absDigree >= 120){
                if(absDigree == 180){
                    correction_ = -1 * int(CATCHING_PWM/2);//180度の場合はより旋回する
                }else{
                    correction_ = -1 * int(CATCHING_PWM/3);
                }
            }else{
                correction_ = 0;
            }
            if(digree == 0){//このフェーズをスキップ
                phase_ = Phase::END_LINE_TRACE;
            }else if(digree < 0){
                curveRunning_->run(correction_,CATCHING_PWM);//右に新地旋回
            }else{
                curveRunning_->run(CATCHING_PWM,correction_);//左に信地旋回
            }
            if(abs(bodyAngleMeasurement_->getResult()) >= absDigree){
                phase_ = Phase::END_LINE_TRACE;
            }
            break;

        //カーブ後のライントレース
        case Phase::END_LINE_TRACE:
            if(absDigree <= 60){//60度より曲がらない場合
                endEdge_ = startEdge_;
            }else if(absDigree <= 105 || absDigree >=150){//60〜105,150〜180度曲がる場合
                if(digree > 0){//左に曲がる場合は右エッジ
                    endEdge_ = LineTraceEdge::RIGHT;
                }else{//右に曲がる場合は左エッジ
                    endEdge_ = LineTraceEdge::LEFT;
                }
            }else if(absDigree <= 135){//135度以上曲がる場合
                if(digree > 0){//左に曲がる場合は左エッジ
                    endEdge_ = LineTraceEdge::LEFT;
                }else{//右に曲がる場合は右エッジ
                    endEdge_ = LineTraceEdge::RIGHT;
                }
            }
            distanceMeasurement_->start(100);
            lineTrace_->run(CATCHING_LINETRACE_PWM,endEdge_);
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::START_LINE_TRACE;
                distanceMeasurement_->reset();
                return true;
            }
            break;
        }
        return false;
    }


}
