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

    bool Catching::run(int dstMm, int degree){


        switch(phase_){

        //色検知するまでライントレース
        case Phase::START_LINE_TRACE:
            startEdge_ = lineTrace_->getEdge();//直前のライントレースのエッジをもらう
            lineTrace_->setPid();
            lineTrace_->setTarget();
            lineTrace_->setEdge(startEdge_);//セットしないとLineTrace._edgeが更新されない
            lineTrace_->run(CATCHING_LINETRACE_PWM,startEdge_);
            if(colorDetection_->isFourColors()){
                phase_ = Phase::STRAIGHT_LITTLE;
            }
            break;

        //タイヤの中心を円周上に
        case Phase::STRAIGHT_LITTLE:
            //タイヤの中心からカラーセンサまでの距離から色検知中に走行した距離を引いた距離走行する
            distanceMeasurement_->start(WHEEL_TO_COLOR_SENSOR - COLOR_DETECTION_DISTANCE);
            straightRunning_->run(10);
            if(distanceMeasurement_->getResult()){
                distanceMeasurement_->reset();
                if(abs(degree) == 180){//後ろに持ち帰る場合は別フェーズへ
                    bodyAngleMeasurement_->setBaseAngle();//角度保存
                    phase_ = Phase::TURN_90;
                }else{
                    phase_ = Phase::PIVOT_FIRST;
                }
            }
            break;

        //最初の旋回
        case Phase::PIVOT_FIRST:
            if(degree == 0 || pivotTurn_->turn(degree / 2,10)){//0度の場合は旋回しない
                phase_ = Phase::STRAIGHT;
             }
             break;

        //二回目の旋回
        case Phase::PIVOT_SECOND:
            if(degree == 0 || pivotTurn_->turn(degree / 2,10)){//0度の場合は旋回しない
                phase_ = Phase::CALC_DISTANCE;
            }
            break;

        //180度専用処理 90度右に信地旋回
        case Phase::TURN_90:
            curveRunning_->run(0,CATCHING_PWM);
            if(bodyAngleMeasurement_->getResult() <= -90){
                bodyAngleMeasurement_->setBaseAngle();
                phase_ = Phase::TURN_270;
            }
            break;

        //180度専用処理 270度左に信地旋回
        case Phase::TURN_270:
            curveRunning_->run(CATCHING_PWM,0);
            if(bodyAngleMeasurement_->getResult() >= 270){
                phase_ = Phase::STRAIGHT_TREAD_DISTANCE;
            }
            break;

        //180度専用処理 走行体のトレッドの距離進む
        case Phase::STRAIGHT_TREAD_DISTANCE:
            distanceMeasurement_->start(TREAD);//measurement::SelfPositionMeasurement::TREAD
            straightRunning_->run(CATCHING_PWM);
            if(distanceMeasurement_->getResult()){
                distanceMeasurement_->reset();
                phase_ = Phase::CALC_DISTANCE;
            }
            break;


        //直進走行
        case Phase::STRAIGHT:
            //円周角の定理から距離を算出
            distanceMeasurement_->start(cos((degree / 2) * M_PI / 180) * DAIZA_DIAMETER);
            straightRunning_->run(CATCHING_PWM);
            if(distanceMeasurement_->getResult()){
                phase_ = Phase::PIVOT_SECOND;
                distanceMeasurement_->reset();
            }
            break;


        case Phase::CALC_DISTANCE:
            //目的ラインの半分　ー　円の半径　進む
            runningDistance_ = dstMm / 2 - DAIZA_DIAMETER / 2;
            if(degree == 0 || abs(degree) == 180){//degree=0,180,-180は補正なし
                phase_ = Phase::END_LINE_TRACE;
            }else if(degree < 0){//左カーブの場合
                if(startEdge_ == LineTraceEdge::RIGHT){//右エッジの場合
                    runningDistance_ += LINE_THICKNESS / 2;
                }else{//左エッジの場合
                    runningDistance_ -= LINE_THICKNESS / 2;
                }
            }else if(degree > 0){//右カーブの場合
                if(startEdge_ == LineTraceEdge::RIGHT){//右エッジの場合
                    runningDistance_ -= LINE_THICKNESS / 2;
                }else{//左エッジの場合
                    runningDistance_ += LINE_THICKNESS / 2;
                }
            }
            phase_ = Phase::END_LINE_TRACE;
            break;


        //カーブ後のライントレース
        case Phase::END_LINE_TRACE:
            //ブロック取得後のラインの半分の距離にタイヤの中心がくるように
            distanceMeasurement_->start(runningDistance_);//エッジの応じた距離走行
            if(abs(degree) == 180){//エッジが逆転する
                if(startEdge_ == LineTraceEdge::RIGHT){
                    endEdge_ = LineTraceEdge::LEFT;
                }else{
                    endEdge_ = LineTraceEdge::RIGHT;
                }
            }else{
                endEdge_ = startEdge_;
            }
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

    //ブロックを置いてバックする
    bool Catching::putBlock(int lineDistance){
        static bool isDaizaDetected = false;
        if(!isDaizaDetected){//台座を検知するまでライントレース
            startEdge_ = lineTrace_->getEdge();
            lineTrace_->setPid();
            lineTrace_->setEdge(startEdge_);
            lineTrace_->run(CATCHING_LINETRACE_PWM,startEdge_);
            if(colorDetection_->isFourColors()){
                isDaizaDetected = true;
            }
        }else{//直前に走行していたラインの中心にタイヤの中心がくるようにバック走行
            distanceMeasurement_->start(int(lineDistance / 2) - DAIZA_DIAMETER / 2 - WHEEL_TO_COLOR_SENSOR + COLOR_DETECTION_DISTANCE);
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
