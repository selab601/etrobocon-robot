#include "ETSumoNeo.h"

using namespace drive;
using namespace detection;
using namespace measurement;

namespace strategy{


    /*
    失敗原因
    ライントレースの開始時の位置が遠くて、ラインに触れた時に直角と認識してしまう
    星取、土俵上の青と緑の検知が鈍い
    下段を攻略中に尻尾がぶつかる危険性がある(旋回方向を変える予定)

    リファクタリング
    SumoPhaseをわかりやすくしたい
    */

    ETSumoNeo::ETSumoNeo(){
        linetrace_              = LineTrace::getInstance();
        straightRunning_        = new StraightRunning();
        climbingRunning_        = new ClimbingRunning();
        extrusionRunning_       = new ExtrusionRunning();
        pivotTurn_              = new PivotTurn();
        curveRunning_           = new CurveRunning();
        lineDetection_          = new LineDetection();
        colorDetection_         = new ColorDetection();
        objectDetection_        = new ObjectDetection();
        rightAngledDetection_   = new RightAngledDetection();
        timeMeasurement_        = new TimeMeasurement();
        distanceMeasurement_    = new DistanceMeasurement();

        hasExecutedSumoPhase_   = false;

        hoshitori_              = Hoshitori::NONE;
        strategyPhase_          = StrategyPhase::HOSHITORI;
        extrusionPhase_         = ExtrusionPhase::LINE_TRACE_1;
        //sumoPhase_              = SumoPhase::EXTRUSION_1;
    }

    bool ETSumoNeo::capture(){
        switch(strategyPhase_){

        //星取取得
        //黒よりにゆっくりライントレースすることで確実に色を取得する
        case StrategyPhase::HOSHITORI:
            linetrace_->run(20,LineTraceEdge::RIGHT,0.5);
            if(hoshitoriDetection(true)){
                strategyPhase_ = StrategyPhase::TURN_1;
            }
            break;

        //左に100度旋回(90度だとラインから少し遠い)
        case StrategyPhase::TURN_1:
            if(pivotTurn_->turn(100)){
                strategyPhase_ = StrategyPhase::STRAIGHT;
            }
            break;

        //ラインに近づくように直進走行
        case StrategyPhase::STRAIGHT:
            if(!hasExecutedSumoPhase_){
                distanceMeasurement_->setTargetDistance(200);//ラインに近づく距離
                distanceMeasurement_->startMeasurement();
                hasExecutedSumoPhase_ = true;
            }
            straightRunning_->run(30);
            if(distanceMeasurement_->getResult()){
                strategyPhase_ = StrategyPhase::LINE_TRACE;
                hasExecutedSumoPhase_ = false;
            }
            break;

        //土俵手前までライントレース
        case StrategyPhase::LINE_TRACE:
            if(!hasExecutedSumoPhase_){
                distanceMeasurement_->setTargetDistance(1000);//土俵手前までの距離
                distanceMeasurement_->startMeasurement();
                hasExecutedSumoPhase_ = true;
            }
            linetrace_->run(30,LineTraceEdge::RIGHT);
            if(distanceMeasurement_->getResult()){
                strategyPhase_ = StrategyPhase::STOP_1;
                hasExecutedSumoPhase_ = false;
            }
            break;

        //新幹線を検知するまで停止
        case StrategyPhase::STOP_1:
            straightRunning_->run(0);
            if(objectDetection_->getResult()){
                ev3_speaker_play_tone ( 500, 100);//音を出す
                strategyPhase_ = StrategyPhase::WAIT_1;
            }
            break;

        //通り過ぎてから1秒間待つ
        case StrategyPhase::WAIT_1:
            if(!hasExecutedSumoPhase_){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(1000);//1秒
                hasExecutedSumoPhase_ = true;
            }
            straightRunning_->run(0);
            if(timeMeasurement_->getResult()){
                strategyPhase_ = StrategyPhase::TURN_2;
                hasExecutedSumoPhase_ = false;
            }
            break;

        //登壇後の動作を安定させるため少し旋回
        case StrategyPhase::TURN_2:
            int turnAngle;
            //星取が青と黄のときは赤から押し出すので左寄りへ
            if(hoshitori_ == Hoshitori::BLUE ||
                hoshitori_ == Hoshitori::YELLOW){
                turnAngle = 6;
            }else{
                turnAngle = -6;
            }
            if(pivotTurn_->turn(turnAngle)){
                strategyPhase_ = StrategyPhase::CLIMB;
            }
            break;

        //登壇走行
        case StrategyPhase::CLIMB:
            if(climbingRunning_->run(40,440)){
                strategyPhase_ = StrategyPhase::WAIT_2;
            }
            break;

        //登壇後に機体がぶれるので安定するまで待つ
        case StrategyPhase::WAIT_2:
            if(!hasExecutedSumoPhase_){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(1000);
                hasExecutedSumoPhase_ = true;
            }
            straightRunning_->run(0);
            if(timeMeasurement_->getResult()){
                strategyPhase_ = StrategyPhase::TURN_3;
                 hasExecutedSumoPhase_ = false;
            }
            break;

        //ラインに近づくように旋回
        case StrategyPhase::TURN_3:
            int turnAngle2;
            if(hoshitori_ == Hoshitori::BLUE ||
                hoshitori_ == Hoshitori::YELLOW){
                turnAngle2 = 70;
            }else{
                turnAngle2 = -70;
            }
            if(pivotTurn_->turn(turnAngle2)){
                strategyPhase_ = StrategyPhase::TURN_4;
            }
            break;

        //ライン検知するまで旋回
        case StrategyPhase::TURN_4:
            int rSpeed,lSpeed;
            if(hoshitori_ == Hoshitori::BLUE||
                hoshitori_ == Hoshitori::YELLOW){
                rSpeed = 15;
                lSpeed = -rSpeed;
            }else{
                rSpeed = -15;
                lSpeed = -rSpeed;
            }
            curveRunning_->run(rSpeed,lSpeed);
            if(lineDetection_->getResult()){
                strategyPhase_ = StrategyPhase::SUMO;
            }

         //相撲
         case StrategyPhase::SUMO:
            if(!hasExecutedSumoPhase_){
                setProcedure();
                hasExecutedSumoPhase_ = true;
            }
            if(executeSumo(sumoProcedure_.front())){
                sumoProcedure_.front();
                hasExecutedSumoPhase_ = false;
            }
            if(sumoProcedure_.empty()){//最後まで終わったら
                strategyPhase_ = StrategyPhase::STOP_2;
            }
            break;

        //新幹線が通り過ぎるまで停止
        case StrategyPhase::STOP_2:
            straightRunning_->run(0);
            if(objectDetection_->getResult()){
                strategyPhase_ = StrategyPhase::WAIT_3;
                ev3_speaker_play_tone(500,100);//音を出す
            }
            break;

        //検知した後すこし待つ
        case StrategyPhase::WAIT_3:
            if(!hasExecutedSumoPhase_){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(1000);
                hasExecutedSumoPhase_ = true;
            }
            straightRunning_->run(0);
            if(timeMeasurement_->getResult()){
                strategyPhase_ = StrategyPhase::GET_OF;
                hasExecutedSumoPhase_ = false;
            }

        //降壇
        case StrategyPhase::GET_OF:
            if(!hasExecutedSumoPhase_){
                distanceMeasurement_->setTargetDistance(300);
                distanceMeasurement_->startMeasurement();
                hasExecutedSumoPhase_ = true;
            }
            straightRunning_->run(30);
            if(distanceMeasurement_->getResult()){
                hasExecutedSumoPhase_ = false;
                return true;
            }
            break;
        }
        return false;
    }

    //読み取った星取りによって押し出す順番を決める
    void ETSumoNeo::setProcedure(){
        //下段(赤,青)以外を押し出す場合
        if(hoshitori_ == Hoshitori::RED || hoshitori_ == Hoshitori::BLUE){
            sumoProcedure_ = {
                SumoPhase::EXTRUSION_FIRST,
                SumoPhase::ACROSS_LINE,
                SumoPhase::TURN_1,
                SumoPhase::UPPER_STAGE,
                SumoPhase::ACROSS_LINE,
                SumoPhase::TURN_2,
                SumoPhase::EXTRUSION_SECOND,
                SumoPhase::ACROSS_LINE,
                SumoPhase::EXTRUSION_THIRD,
                SumoPhase::TURN_1
            };

            //赤の場合
            if(hoshitori_ == Hoshitori::RED){
                firstWrestlerColor_ = Hoshitori::BLUE;
                secondWrestlerColor_ = Hoshitori::GREEN;
                thirdWrestlerColor_ = Hoshitori::YELLOW;
                upperStageEdge_ = LineTraceEdge::LEFT;
                angleTowardTop_ = -85;
                angleTowardWrestler_ = 85;

            //青の場合
            }else{
                firstWrestlerColor_ = Hoshitori::RED;
                secondWrestlerColor_ = Hoshitori::YELLOW;
                thirdWrestlerColor_ = Hoshitori::GREEN;
                upperStageEdge_ = LineTraceEdge::RIGHT;
                angleTowardTop_ = 85;
                angleTowardWrestler_ = -85;
            }

        //上段(黄,緑)以外を押し出す場合
        }else{
            sumoProcedure_ = {
                SumoPhase::EXTRUSION_FIRST,
                SumoPhase::ACROSS_LINE,
                SumoPhase::EXTRUSION_SECOND,
                SumoPhase::ACROSS_LINE,
                SumoPhase::TURN_1,
                SumoPhase::UPPER_STAGE,
                SumoPhase::ACROSS_LINE,
                SumoPhase::TURN_2,
                SumoPhase::EXTRUSION_THIRD,
                SumoPhase::TURN_1
            };

            //黄の場合
            if(hoshitori_ == Hoshitori::YELLOW){
                firstWrestlerColor_ = Hoshitori::RED;
                secondWrestlerColor_ = Hoshitori::BLUE;
                thirdWrestlerColor_ = Hoshitori::GREEN;
                upperStageEdge_ = LineTraceEdge::RIGHT;
                angleTowardTop_ = -85;
                angleTowardWrestler_ = -85;

            //緑の場合
            }else{
                firstWrestlerColor_ = Hoshitori::BLUE;
                secondWrestlerColor_ = Hoshitori::RED;
                thirdWrestlerColor_ = Hoshitori::YELLOW;
                upperStageEdge_ = LineTraceEdge::LEFT;
                angleTowardTop_ = 85;
                angleTowardWrestler_ = 85;
            }
        }
    }

    //相撲...登壇後から降壇前まで
    bool ETSumoNeo::executeSumo(SumoPhase sumoPhase){
        switch(sumoPhase){

        //一人目の力士を押し出す
        case SumoPhase::EXTRUSION_FIRST:
            return extrusion(firstWrestlerColor_);

        //二人目の力士を押し出す
        case SumoPhase::EXTRUSION_SECOND:
            return extrusion(secondWrestlerColor_);

        //三人目の力士を押し出す
        case SumoPhase::EXTRUSION_THIRD:
            return extrusion(thirdWrestlerColor_);

        //直角をまたぐ走行
        case SumoPhase::ACROSS_LINE:
            if(!hasExecutedSumoPhase_){
                distanceMeasurement_->setTargetDistance(50);
                distanceMeasurement_->startMeasurement();
                hasExecutedSumoPhase_ = true;
            }
            straightRunning_->run(20);
            return distanceMeasurement_->getResult();


        //上(後段する方向)を向くように回転
        case SumoPhase::TURN_1:
            return pivotTurn_->turn(angleTowardTop_);

        //力士を向くように回転
        case SumoPhase::TURN_2:
            return pivotTurn_->turn(angleTowardWrestler_);

        //上段までライントレース
        case SumoPhase::UPPER_STAGE:
            linetrace_->run(20,upperStageEdge_);
            return rightAngledDetection_->getResult(4.0);

        }
        return false;
    }

    //中央線から力士を押し出して戻って来る
    bool ETSumoNeo::extrusion(Hoshitori hoshitori){
        int rSpeed;     //右タイヤスピード
        int lSpeed;     //左タイヤスピード
        //int turnAngle1 = 0; //旋回角度
        int turnAngle2;
        LineTraceEdge edge,edge2;
        //左側(赤,黄)と右側(青,緑)それぞれ同じ動作をする
        switch(hoshitori){
        //左側
        case Hoshitori::RED:
        case Hoshitori::YELLOW:
            rSpeed = 15;
            lSpeed = -rSpeed;
            //turnAngle1 = 70;
            turnAngle2 = -170;
            edge = LineTraceEdge::RIGHT;
            edge2 = LineTraceEdge::LEFT;
            break;

        //右側
        case Hoshitori::BLUE:
        case Hoshitori::GREEN:
            rSpeed = -15;
            lSpeed = -rSpeed;
            //turnAngle1 = -70;
            turnAngle2 = 170;

            edge = LineTraceEdge::LEFT;
            edge2 = LineTraceEdge::RIGHT;
            break;

        default: return false;
        }

        switch(extrusionPhase_){
        //力士までライントレース
        case ExtrusionPhase::LINE_TRACE_1:
            linetrace_->run(15,edge);
            if(hoshitoriDetection()){
                extrusionPhase_ = ExtrusionPhase::EXTRUSION;
            }
            break;

        //押し出し走行
        case ExtrusionPhase::EXTRUSION:
            if(extrusionRunning_->run(SUMO_EXTRUSION_SPEED,SUMO_EXTRUSION_DISTANCE)){
                extrusionPhase_ = ExtrusionPhase::TURN_1;
            }
            break;

        //星取を誤検知しないように170度旋回しておきたい
        case ExtrusionPhase::TURN_1:
            if(pivotTurn_->turn(turnAngle2)){
                extrusionPhase_ = ExtrusionPhase::TURN_2;
            }
            break;

        //ラインまで旋回
        case ExtrusionPhase::TURN_2:
            curveRunning_->run(-rSpeed,-lSpeed);
            if(lineDetection_->getResult()){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(1000);
                extrusionPhase_ = ExtrusionPhase::LINE_TRACE_2;
            }
            break;

        //直角までライントレース
        case ExtrusionPhase::LINE_TRACE_2:
            linetrace_->run(20,edge2);
            //直角誤検知しないように
            if(!timeMeasurement_->getResult()){
                break;
            }
            if(rightAngledDetection_->getResult()){
                extrusionPhase_ = ExtrusionPhase::LINE_TRACE_1;//状態を戻しておく
                return true;
            }
            break;
        }
        return false;
    }

    bool ETSumoNeo::hoshitoriDetection(bool saveHoshitori){
        // colorid_t nowColor = COLOR_NONE;
        colorid_t nowColor = colorDetection_->getResult();
        if(nowColor == COLOR_BLUE || nowColor == COLOR_RED ||
        nowColor == COLOR_GREEN || nowColor == COLOR_YELLOW){
            //星取保存
            if(saveHoshitori){
                switch(nowColor){
                case COLOR_RED:
                    hoshitori_ = Hoshitori::RED; break;
                case COLOR_BLUE:
                    hoshitori_ = Hoshitori::BLUE; break;
                case COLOR_YELLOW:
                    hoshitori_ = Hoshitori::YELLOW; break;
                case COLOR_GREEN:
                    hoshitori_ = Hoshitori::GREEN; break;
                default: return false;
                }
            }
            return true;
        }else{
            return false;
        }
    }

}
