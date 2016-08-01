#include "ETSumoNeo.h"

using namespace drive;
using namespace detection;
using namespace measurement;

namespace strategy{
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

        hasExecutedPhase_       = false;
        strategySuccess_        = false;

        hoshitori_              = Hoshitori::NONE;
        extrusionPhase_         = ExtrusionPhase::START_LINE_TRACE;
    }

    bool ETSumoNeo::capture(){
        static unsigned int procedureNumber = 0;
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executeStrategy(strategyProcedure_[procedureNumber])){
                procedureNumber++;
                hasExecutedPhase_ = false;//フラグを戻しておく
            }
        }
        if(procedureNumber == strategyProcedure_.size()){//最後まで終わったら
            strategySuccess_ = true;
            return true;
        }
        return strategySuccess_;
    }

    //戦略手順を実行する
    bool ETSumoNeo::executeStrategy(StrategyPhase strategyPhase){
        switch(strategyPhase){
        //星取取得
        case StrategyPhase::HOSHITORI:
            linetrace_->run(20,LineTraceEdge::RIGHT);
            return hoshitoriDetection(true);

        //左に100度旋回(90度だとラインから少し遠い)
        case StrategyPhase::TURN_LEFT:
            return pivotTurn_->turn(100);


        //ラインに近づくように直進走行
        case StrategyPhase::STRAIGHT:
            startDistanceMeasurement(190);
            straightRunning_->run(30);
            return distanceMeasurement_->getResult();

        //土俵手前までライントレース
        case StrategyPhase::LINE_TRACE:
            startDistanceMeasurement(1050);
            linetrace_->run(30,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        //新幹線を検知するまで停止
        case StrategyPhase::STOP:
            straightRunning_->run(0);
            if(objectDetection_->getResult()){
                ev3_speaker_play_tone ( 500, 100);//音を出す
                return true;
            }
            return false;

        //通り過ぎてから1秒間待つ
        case StrategyPhase::WAIT_1_SEC:
            startTimeMeasurement(1000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //登壇後の動作を安定させるため少し旋回
        /////////////完全に安定させるために修正が必要/////////////
        case StrategyPhase::TURN_LITTLE:
            //星取が青と黄のときは赤から押し出すので左寄りへ
            if(hoshitori_ == Hoshitori::BLUE ||
                hoshitori_ == Hoshitori::GREEN){
                return pivotTurn_->turn(5);
            }else{
                return pivotTurn_->turn(-5);
            }
            return false;

        //登壇走行
        case StrategyPhase::CLIMB:
            return climbingRunning_->run(40,410);

         //相撲
         case StrategyPhase::SUMO:
            return captureSumo();

        //検知した後すこし待つ
        case StrategyPhase::WAIT_2_SEC:
            startTimeMeasurement(2000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //降壇
        case StrategyPhase::GET_OF:
            startDistanceMeasurement(300);//もっと必要
            straightRunning_->run(30);
            return distanceMeasurement_->getResult();

        default: return false;
        }
    }

    bool ETSumoNeo::captureSumo(){
        static unsigned int procedureNumber = 0;
        //星取が下段の場合
        if(hoshitori_ == Hoshitori::RED || hoshitori_ == Hoshitori::BLUE){
            if(executeSumo(sumoProcedureRorB_[procedureNumber])){
                procedureNumber++;
                hasExecutedPhase_ = false;
            }
            return procedureNumber == sumoProcedureRorB_.size();
        }else{//星取が上段の場合
            if(executeSumo(sumoProcedureYorG_[procedureNumber])){
                procedureNumber++;
                hasExecutedPhase_ = false;
            }
            return procedureNumber == sumoProcedureYorG_.size();
        }
        return false;
    }

    //相撲...登壇後から降壇前まで
    bool ETSumoNeo::executeSumo(SumoPhase sumoPhase){
        static bool initialized = false;        //初期化を行ったかどうか
        static Hoshitori firstWrestlerColor;    //一人目の力士の色
        static Hoshitori secondWrestlerColor;   //二人目の力士の色
        static Hoshitori thirdWrestlerColor;    //三人目の力士の色
        static LineTraceEdge upperStageEdge;    //上段に移動するライントレースのエッジ
        static int angleTowardTop;              //上段を向く角度
        static int angleTowardSide;             //力士の方向を向く角度
        static int angleTowardLine;             //ラインに向かう角度
        static int rSpeed;                      //右タイヤスピード
        static int lSpeed;                      //左タイヤスピード

        if(!initialized){
            switch(hoshitori_){
            //赤の場合
            case Hoshitori::RED:
                firstWrestlerColor = Hoshitori::BLUE;
                secondWrestlerColor = Hoshitori::YELLOW;
                thirdWrestlerColor = Hoshitori::GREEN;
                upperStageEdge = LineTraceEdge::LEFT;
                angleTowardTop = -85;
                angleTowardSide = 85;
                angleTowardLine = -50;
                rSpeed = -15;
                lSpeed = -rSpeed;
                break;
            //青の場合
            case Hoshitori::BLUE:
                firstWrestlerColor = Hoshitori::RED;
                secondWrestlerColor = Hoshitori::GREEN;
                thirdWrestlerColor = Hoshitori::YELLOW;
                upperStageEdge = LineTraceEdge::RIGHT;
                angleTowardTop = 85;
                angleTowardSide = -85;
                angleTowardLine = 50;
                rSpeed = 15;
                lSpeed = -rSpeed;
                break;
            //黄の場合
            case Hoshitori::YELLOW:
                firstWrestlerColor = Hoshitori::BLUE;
                secondWrestlerColor = Hoshitori::RED;
                thirdWrestlerColor = Hoshitori::GREEN;
                upperStageEdge = LineTraceEdge::RIGHT;
                angleTowardTop = 85;
                angleTowardSide = -85;
                angleTowardLine = -50;
                rSpeed = -15;
                lSpeed = -rSpeed;
                break;
            //緑の場合
            case Hoshitori::GREEN:
                firstWrestlerColor = Hoshitori::RED;
                secondWrestlerColor = Hoshitori::BLUE;
                thirdWrestlerColor = Hoshitori::YELLOW;
                upperStageEdge = LineTraceEdge::LEFT;
                angleTowardTop = -85;
                angleTowardSide = 85;
                angleTowardLine = 50;
                rSpeed = 15;
                lSpeed = -rSpeed;
                break;
            default: return false;
            }
            initialized = true;
        }

        switch(sumoPhase){
        //ある程度回転...ライン上にいることもあるので
        case SumoPhase::FIRST_TURN:
            return pivotTurn_->turn(angleTowardLine);

        //ライン検知するまで旋回
        case SumoPhase::SECOND_TURN:
            curveRunning_->run(rSpeed,lSpeed);
            return lineDetection_->getResult();

        //一人目の力士を押し出す
        case SumoPhase::FIRST_EXTRUSION:
            return extrusion(firstWrestlerColor);

        //二人目の力士を押し出す
        case SumoPhase::SECOND_EXTRUSION:
            return extrusion(secondWrestlerColor);

        //三人目の力士を押し出す
        case SumoPhase::THIRD_EXTRUSION:
            return extrusion(thirdWrestlerColor);

        //直角をまたぐ走行
        case SumoPhase::ACROSS_LINE:
            startDistanceMeasurement(40);
            straightRunning_->run(15);
            if(distanceMeasurement_->getResult()){
                ev3_speaker_play_tone(500,100);//音を出す
                return true;
            }
            return false;

        //上(後段する方向)を向くように回転
        case SumoPhase::TURN_TOP:
            return pivotTurn_->turn(angleTowardTop);

        //横(力士)を向くように回転
        case SumoPhase::TURN_SIDE:
            return pivotTurn_->turn(angleTowardSide);

        //上段までライントレース
        case SumoPhase::UPPER_STAGE:
            startTimeMeasurement(1000);
            linetrace_->run(20,upperStageEdge);
            if(!timeMeasurement_->getResult()){break;}//開始直後は直角無視
            return rightAngledDetection_->getResult(4.0);

        default: return false;
        }
        return false;
    }

    //中央線から力士を押し出して戻って来る
    bool ETSumoNeo::extrusion(Hoshitori wrestlerColor){
        static bool initialized = false;
        static int rSpeed;     //右タイヤスピード
        static int lSpeed;     //左タイヤスピード
        static int turnAngle;  //戻るための旋回角度
        static LineTraceEdge startEdge,endEdge;//往復のライントレースのエッジ

        if(!initialized){
            //左側(赤,黄)と右側(青,緑)それぞれ同じ動作をする
            switch(wrestlerColor){
            //左側
            case Hoshitori::RED:
            case Hoshitori::YELLOW:
                rSpeed = 15;
                lSpeed = -rSpeed;
                turnAngle = -160;
                startEdge = LineTraceEdge::RIGHT;
                endEdge = LineTraceEdge::LEFT;
                break;

            //右側
            case Hoshitori::BLUE:
            case Hoshitori::GREEN:
                rSpeed = -15;
                lSpeed = -rSpeed;
                turnAngle = 160;
                startEdge = LineTraceEdge::LEFT;
                endEdge = LineTraceEdge::RIGHT;
                break;

            default: return false;
            }
            initialized = true;
        }

        switch(extrusionPhase_){
        //力士までライントレース
        case ExtrusionPhase::START_LINE_TRACE:
            linetrace_->run(15,startEdge);
            if(hoshitoriDetection()){
                extrusionPhase_ = ExtrusionPhase::EXTRUSION;
            }
            break;

        //押し出し走行
        case ExtrusionPhase::EXTRUSION:
            if(extrusionRunning_->run(SUMO_EXTRUSION_SPEED,SUMO_EXTRUSION_DISTANCE)){
                extrusionPhase_ = ExtrusionPhase::FIRST_TURN;
            }
            break;

        //星取を誤検知しないように170度旋回しておきたい
        case ExtrusionPhase::FIRST_TURN:
            if(pivotTurn_->turn(turnAngle)){
                extrusionPhase_ = ExtrusionPhase::SECOND_TURN;
            }
            break;

        //ラインまで旋回
        case ExtrusionPhase::SECOND_TURN:
            curveRunning_->run(-rSpeed,-lSpeed);
            if(lineDetection_->getResult()){
                timeMeasurement_->setBaseTime();
                timeMeasurement_->setTargetTime(1000);
                extrusionPhase_ = ExtrusionPhase::END_LINE_TRACE;
            }
            break;

        //直角までライントレース
        case ExtrusionPhase::END_LINE_TRACE:
            linetrace_->run(20,endEdge);
            if(!timeMeasurement_->getResult()){break;}//直角誤検知しないように
            if(rightAngledDetection_->getResult()){
                extrusionPhase_ = ExtrusionPhase::START_LINE_TRACE;//状態を戻しておく
                initialized = false;
                return true;
            }
            break;
        }
        return false;
    }
    /*
    変更予定
    ・白、黒以外だったら検知するようにする
    ・赤、黄、緑以外だったら青と判定するようにする
    */
    bool ETSumoNeo::hoshitoriDetection(bool saveHoshitori){
        static colorid_t nowColor = COLOR_NONE;
        nowColor = colorDetection_->getResult();
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

    void ETSumoNeo::startDistanceMeasurement(int distance){
        if(!hasExecutedPhase_){
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            hasExecutedPhase_ = true;
        }
    }

    void ETSumoNeo::startTimeMeasurement(int time){
        if(!hasExecutedPhase_){
            timeMeasurement_->setBaseTime();
            timeMeasurement_->setTargetTime(time);
            hasExecutedPhase_ = true;
        }
    }

}
