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
        selfPositionEstimation_ = SelfPositionEstimation::getInstance();
    }

    bool Catching::run(int currentMm, int dstMm, int degree){
        static int x;//目的地の座標
        static int y;
        static double rad;

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

        case Phase::INIT:
            selfPositionEstimation_->startMeasure();
            if(absDegree == 180){
                x = selfPositionEstimation_->getLocationX();
            }else{
                rad  = degree * M_PI / 180;
                x = dstMm * cos(rad) / 2;
                y = dstMm * sin(rad) / 2;
                x += currentMm /2 ;
                dstDegree_ = atan2(y,x) * 1800 / M_PI;
            }
            phase_ = Phase::START_LINE_TRACE;
            break;

        //色検知するまでライントレース
        case Phase::START_LINE_TRACE:
            lineTrace_->setPid();
            lineTrace_->setTarget();
            lineTrace_->setEdge(startEdge_);//セットしないとLineTrace._edgeが更新されない
            lineTrace_->run(CATCHING_LINETRACE_PWM,startEdge_);
            if(colorDetection_->isFourColors()){
                bodyAngleMeasurement_->setBaseAngle();//検知した時の角度保存
                phase_ = Phase::STRAIGHT;
            }
            break;



        //直進走行
        case Phase::STRAIGHT:
            if(absDegree <= 90){//0,30,45,60,90の場合
                if(absDegree == 0){//0度の場合は多めに直進
                    distanceMeasurement_->start(110);
                }else if(absDegree == 90){//90度の場合は少なめに直進
                    distanceMeasurement_->start(30);
                }else if( ( (degree == -30 || degree == -45)  && startEdge_ == LineTraceEdge::LEFT)
                       || ( (degree == 30  || degree == 45)  && startEdge_ == LineTraceEdge::RIGHT)){//外側で30,45角度の場合
                    distanceMeasurement_->start(60);
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
            if(absDegree >= 120){
                if(absDegree == 180){
                    correction_ = -1 * int(CATCHING_PWM/2);//180度の場合はより旋回する
                }else{
                    correction_ = -1 * int(CATCHING_PWM/3);
                }
            }else{
                correction_ = 0;
            }
            if(degree == 0){//このフェーズをスキップ
                phase_ = Phase::END_LINE_TRACE;
            }else if(degree < 0){
                curveRunning_->run(correction_,CATCHING_PWM);//右に新地旋回
            }else{
                curveRunning_->run(CATCHING_PWM,correction_);//左に信地旋回
            }
            if(abs(bodyAngleMeasurement_->getResult()) >= absDegree){
                phase_ = Phase::END_LINE_TRACE;
            }
            break;

        //カーブ後のライントレース
        case Phase::END_LINE_TRACE:
            if(absDegree <= 60){//60度より曲がらない場合
                endEdge_ = startEdge_;
            }else if(absDegree <= 105 || absDegree >=150){//60〜105,150〜180度曲がる場合
                if(degree > 0){//左に曲がる場合は右エッジ
                    endEdge_ = LineTraceEdge::RIGHT;
                }else{//右に曲がる場合は左エッジ
                    endEdge_ = LineTraceEdge::LEFT;
                }
            }else if(absDegree <= 135){//135度以上曲がる場合
                if(degree > 0){//左に曲がる場合は左エッジ
                    endEdge_ = LineTraceEdge::LEFT;
                }else{//右に曲がる場合は右エッジ
                    endEdge_ = LineTraceEdge::RIGHT;
                }
            }
            lineTrace_->setEdge(endEdge_);
            lineTrace_->run(CATCHING_LINETRACE_PWM,endEdge_);
            if(absDegree == 180){
                if( x >= selfPositionEstimation_->getLocationX()){
                    phase_ = Phase::INIT;
                    return true;
                }
            }else{
                if(abs(dstDegree_) <= abs(selfPositionEstimation_->getPolarTheta10())){
                    phase_ = Phase::INIT;
                    return true;
                }
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
            //円の半径5cm,カラーセンサの中心からタイヤの中心までの距離4cm,色検知中に走ってしまう距離1.5cm(pwm20)
            distanceMeasurement_->start(int(lineDistance/2)-50-40+15);//ラインの中心にタイヤの中心がくるように
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
