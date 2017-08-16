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
        polarRunning_ = new PolarRunning();
        colorDetection_ = new ColorDetection();
        distanceMeasurement_ = new DistanceMeasurement();
        selfPositionEstimation_ = SelfPositionEstimation::getInstance();

        polarRunning_->centerPivot(false);//信地旋回する
    }

    bool Catching::run(int dstMm, int degree){


        switch(phase_){

        //色検知するまでライントレース
        case Phase::START_LINE_TRACE:
            lineTrace_->useHsv(true);           // HSV値を使ってライントレース
            startEdge_ = lineTrace_->getEdge();//直前のライントレースのエッジをもらう
            lineTrace_->setPid(LineTracePid::FAST);
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
            straightRunning_->run(CATCHING_LINETRACE_PWM);
            if(distanceMeasurement_->getResult()){
                distanceMeasurement_->reset();
                if(abs(degree) >= 175){//後ろに持ち帰る場合は別フェーズへ(本来180が理想だが179が来るらしい)
                    phase_ = Phase::TURN_90;
                }else if(abs(degree) <= 5){
                    phase_ = Phase::STRAIGHT;
                }else{
                    phase_ = Phase::PIVOT_FIRST;
                }
            }
            break;

        //最初の旋回
        case Phase::PIVOT_FIRST:
            ev3_speaker_play_tone ( 500, 100);//音を出す
            if(pivotTurn_->turn(degree / 2,CATCHING_PWM)){
                phase_ = Phase::STRAIGHT;
             }
             break;

        //二回目の旋回
        case Phase::PIVOT_SECOND:
            ev3_speaker_play_tone ( 700, 100);//音を出す
            if(pivotTurn_->turn(degree / 2,CATCHING_PWM)){
                phase_ = Phase::CALC_DISTANCE;
            }
            break;

        //180度専用処理 90度右に信地旋回
        case Phase::TURN_90:
            ev3_speaker_play_tone ( 500, 100);//音を出す
            if(polarRunning_->bodyTurn(-900,CATCHING_180_PWM)){
                phase_ = Phase::TURN_270_1;
            }
            break;

        //180度専用処理 270度左に信地旋回(135度を2回)
        case Phase::TURN_270_1:
            ev3_speaker_play_tone ( 600, 100);//音を出す
            if(polarRunning_->bodyTurn(1350,CATCHING_180_PWM)){
                phase_ = Phase::TURN_270_2;
            }
            break;

        case Phase::TURN_270_2:
            ev3_speaker_play_tone ( 600, 100);//音を出す
            if(polarRunning_->bodyTurn(1350,CATCHING_180_PWM)){
                phase_ = Phase::STRAIGHT_TREAD_DISTANCE;
            }
            break;

        //180度専用処理 走行体のトレッドの距離進む
        case Phase::STRAIGHT_TREAD_DISTANCE:
            ev3_speaker_play_tone ( 700, 100);//音を出す
            distanceMeasurement_->start(TREAD);//measurement::SelfPositionMeasurement::TREAD
            straightRunning_->run(CATCHING_180_PWM);
            if(distanceMeasurement_->getResult()){
                distanceMeasurement_->reset();
                phase_ = Phase::CALC_DISTANCE;
            }
            break;


        //直進走行
        case Phase::STRAIGHT:
            ev3_speaker_play_tone ( 600, 100);//音を出す
            //円周角の定理から距離を算出
            distanceMeasurement_->start(cos((degree / 2) * M_PI / 180) * DAIZA_DIAMETER);
            straightRunning_->run(CATCHING_PWM);
            if(distanceMeasurement_->getResult()){
                if(abs(degree) <= 5){
                    phase_ = Phase::CALC_DISTANCE;
                }else{
                    phase_ = Phase::PIVOT_SECOND;
                }
                distanceMeasurement_->reset();
            }
            break;


        case Phase::CALC_DISTANCE:
            //目的ラインの半分　ー　円の半径　進む
            runningDistance_ = dstMm / 2 - DAIZA_DIAMETER / 2;
            if(abs(degree) <= 5 || abs(degree) >= 175){//degree=0,180,-180は補正なし
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

            if(abs(degree) < 85){//エッジそのまま
                endEdge_ = startEdge_;
            }else if(abs(degree) < 95){//90度付近の場合
                if(startEdge_ == LineTraceEdge::RIGHT){//右エッジ＆右カーブの時エッジ変える
                    if(degree < 0){
                        endEdge_ = LineTraceEdge::LEFT;
                    }else{
                        endEdge_ = startEdge_;
                    }
                }else{//左エッジ＆左カーブの時エッジ変える
                    if(degree > 0){
                        endEdge_ = LineTraceEdge::RIGHT;
                    }else{
                        endEdge_ = startEdge_;
                    }
                }
            }else{//エッジ逆転
                if(startEdge_ == LineTraceEdge::RIGHT){
                    endEdge_ = LineTraceEdge::LEFT;
                }else{
                    endEdge_ = LineTraceEdge::RIGHT;
                }
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
            lineTrace_->setPid(LineTracePid::MID);
            lineTrace_->setEdge(startEdge_);
            lineTrace_->run(20,startEdge_);//置くときはゆっくり
            if(colorDetection_->isFourColors()){
                isDaizaDetected = true;
            }
        }else{//直前に走行していたラインの中心にタイヤの中心がくるようにバック走行
            distanceMeasurement_->start(int(lineDistance / 2) - DAIZA_DIAMETER / 2 - WHEEL_TO_COLOR_SENSOR + COLOR_DETECTION_DISTANCE);
            straightRunning_->run(-20);//速いと急バックによる車体のぶれとモーターの個体差によるずれがあるため
            if(distanceMeasurement_->getResult()){
                isDaizaDetected =false;//フラグを戻しておく
                distanceMeasurement_->reset();
                return true;
            }
        }
        return false;
    }


}
