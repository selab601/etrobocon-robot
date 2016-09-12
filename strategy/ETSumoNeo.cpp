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
        bodyAngleMeasurement_   = new BodyAngleMeasurement();

        hasExecutedPhase_       = false;
        strategySuccess_        = false;
        sumoSuccess_            = false;

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
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        if(procedureNumber == strategyProcedure_.size()){//最後まで終わったら
            strategySuccess_ = true;
            straightRunning_->run(0);//あとで消す
            return true;
        }
        return strategySuccess_;
    }

    //戦略手順を実行する
    bool ETSumoNeo::executeStrategy(StrategyPhase strategyPhase){
        switch(strategyPhase){

        //車体角度保存
        case StrategyPhase::INIT:
            bodyAngleMeasurement_->setBaseAngle();//始めの角度をセット
            return true;

        //星取取得
        case StrategyPhase::HOSHITORI:
            linetrace_->run(20,LineTraceEdge::RIGHT);
            return hoshitoriDetection(true);

        case StrategyPhase::SET_VALUE:
            setValue();
            return true;

        //すこしバック
        case StrategyPhase::BACK:
            startDistanceMeasurement(100);
            straightRunning_->run(-20);
            return distanceMeasurement_->getResult();

        //左に80度旋回
        case StrategyPhase::TURN_LEFT:
            return pivotTurn_->turn(80);

        //ラインに近づくように直進走行
        case StrategyPhase::STRAIGHT:
            startDistanceMeasurement(150);
            straightRunning_->run(20);
            return lineDetection_->getResult() || distanceMeasurement_->getResult();

        //土俵を向くまでライントレース
        case StrategyPhase::LINE_TRACE:
            startDistanceMeasurement(900);
            linetrace_->run(30,LineTraceEdge::RIGHT);
            //距離検知or車体角度が土俵を向いたらtrue
            return distanceMeasurement_->getResult() || bodyAngleMeasurement_->getResult() >= 180;

        //すこしライントレース
        case StrategyPhase::LINE_TRACE_LITTLE:
            startDistanceMeasurement(200);
            linetrace_->run(30,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        //新幹線を検知するまで停止
        case StrategyPhase::STOP:
            straightRunning_->run(0);
            return objectDetection_->getResult();

        //通り過ぎてから1秒間待つ
        case StrategyPhase::WAIT_1_SEC:
            startTimeMeasurement(1000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        //登壇後の動作を安定させるため少し旋回
        case StrategyPhase::TURN_LITTLE:
            return pivotTurn_->turn(climbBeforeLittleAngle_);

        //登壇走行
        case StrategyPhase::CLIMB:
            return climbingRunning_->run(40,550);

        //横を向くまで旋回
        case StrategyPhase::TURN_TO_SIDE:
            return pivotTurn_->turn(climbAfterSideFaceAngle_);

        //ラインまでバック
        case StrategyPhase::BACK_TO_LINE:
            straightRunning_->run(-15);
            return lineDetection_->getResult();

        //4cm進む
        case StrategyPhase::STRAIGHT_4_CM:
            startDistanceMeasurement(45);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //下を向くように旋回
        case StrategyPhase::TURN_TO_DOWN:
            return turn(isRightRotation_,10);

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
            startDistanceMeasurement(400);
            straightRunning_->run(30);
            return distanceMeasurement_->getResult();

        //ライン検知
        case StrategyPhase::LINE_DETECTION:
            curveRunning_->run(20,10);
            return lineDetection_->getResult();

        //ライン復帰
        case StrategyPhase::LINE_RETURN:
            startDistanceMeasurement(700);
            linetrace_->run(30,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        /*以下安全なライン復帰*/
        case StrategyPhase::TURN_RIGHT_90:
            return pivotTurn_->turn(-90);

        case StrategyPhase::TURN_LEFT_90:
            return pivotTurn_->turn(90);

        case StrategyPhase::LEAVE_FROM_LINE:
            startDistanceMeasurement(100);
            straightRunning_->run(20);
            return distanceMeasurement_->getResult();

        case StrategyPhase::APPROACH_TO_LINE:
            straightRunning_->run(-20);
            return lineDetection_->getResult();

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
                bodyAngleMeasurement_->setBaseAngle();
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
            if(procedureNumber == sumoProcedureRorB_.size()){
                sumoSuccess_ = true;
            }
        }else{//星取が上段の場合
            if(executeSumo(sumoProcedureYorG_[procedureNumber])){
                procedureNumber++;
                hasExecutedPhase_ = false;
                bodyAngleMeasurement_->setBaseAngle();
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
            if(procedureNumber == sumoProcedureYorG_.size()){
                sumoSuccess_ = true;
            }
        }
        return sumoSuccess_;
    }

    //相撲...登壇後から降壇前まで
    bool ETSumoNeo::executeSumo(SumoPhase sumoPhase){
        switch(sumoPhase){

        //一人目の力士を押し出す
        case SumoPhase::FIRST_EXTRUSION:
            return extrusion(firstWrestlerColor_);

        //二人目の力士を押し出す
        case SumoPhase::SECOND_EXTRUSION:
            return extrusion(secondWrestlerColor_);

        //三人目の力士を押し出す
        case SumoPhase::THIRD_EXTRUSION:
            return extrusion(thirdWrestlerColor_);

        //直角をまたぐ走行
        case SumoPhase::ACROSS_LINE:
            startDistanceMeasurement(50);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();

        //一回目の旋回
        case SumoPhase::FIRST_TURN:
            return pivotTurn_->turn(firstTurnAngle_);

        //二回目の旋回
        case SumoPhase::SECOND_TURN:
            return pivotTurn_->turn(secondTurnAngle_);

        //三回目の旋回
        case SumoPhase::THIRD_TURN:
            return pivotTurn_->turn(thirdTurnAngle_);

        //旋回すると尻尾が新幹線にぶつかるのでカーブをして上を向く
        case SumoPhase::CURVE_TOP:
            return curve(isRightCurve_,20);

        //上段までライントレース
        case SumoPhase::UPPER_STAGE:
            static bool isTimeDetected = false;
            startTimeMeasurement(1000);
            linetrace_->run(20,upperStageEdge_,0.4);
            if(timeMeasurement_->getResult()){
                isTimeDetected = true;
            }
            return isTimeDetected && rightAngledDetection_->getResult(4.0);

        //下段までライントレース
        case SumoPhase::DOWN_STAGE:
            static bool isTimeDetected2 = false;
            startTimeMeasurement(500);
            linetrace_->run(20,downStageEdge_,0.4);
            if(timeMeasurement_->getResult()){
                isTimeDetected2 = true;
            }
            return isTimeDetected2 && rightAngledDetection_->getResult(4.0);

        //3cm直進
        case SumoPhase::STRAIGHT_3_CM:
            startDistanceMeasurement(20);
            straightRunning_->run(20);
            return distanceMeasurement_->getResult();

        //青と緑の時のみ行う処理
        case SumoPhase::CORRECTION:
            return correction();

        default: return false;
        }
        return false;
    }

    //中央線からブロックを押し出して戻って来る
    bool ETSumoNeo::extrusion(Hoshitori blockColor){
        static bool initialized = false;//初期化したかどうか
        static bool isRightCurve;       //右回転するかどうか
        static int turnAngle;           //戻るための旋回角度
        static LineTraceEdge startEdge,endEdge;//往復のライントレースのエッジ
        static bool isTimeDetected;

        if(!initialized){
            isTimeDetected = false;
            //左側(赤,黄)と右側(青,緑)それぞれ同じ動作をする
            switch(blockColor){
            case Hoshitori::RED:
                isRightCurve = true;
                turnAngle = 200;
                startEdge = LineTraceEdge::RIGHT;
                endEdge = LineTraceEdge::LEFT;
            break;

            case Hoshitori::YELLOW:
                isRightCurve = true;
                turnAngle = -160;
                startEdge = LineTraceEdge::RIGHT;
                endEdge = LineTraceEdge::LEFT;
                break;

            case Hoshitori::BLUE:
                isRightCurve = false;
                turnAngle = -200;
                startEdge = LineTraceEdge::LEFT;
                endEdge = LineTraceEdge::RIGHT;
                break;

            case Hoshitori::GREEN:
                isRightCurve = false;
                turnAngle = 160;
                startEdge = LineTraceEdge::LEFT;
                endEdge = LineTraceEdge::RIGHT;
                break;

            default: return false;
            }
            initialized = true;
        }

        switch(extrusionPhase_){
        //ブロックまでライントレース
        case ExtrusionPhase::START_LINE_TRACE:
            startTimeMeasurement(500);
            linetrace_->run(15,startEdge,0.4);//黒よりに変更
            if(timeMeasurement_->getResult()){
                isTimeDetected = true;
            }
            if(isTimeDetected && hoshitoriDetection()){
                extrusionPhase_ = ExtrusionPhase::EXTRUSION;
                isTimeDetected = false;
                hasExecutedPhase_ = false;
            }
            break;

        //押し出し走行
        case ExtrusionPhase::EXTRUSION:
            if(extrusionRunning_->run(SUMO_EXTRUSION_SPEED,SUMO_EXTRUSION_DISTANCE)){
                extrusionPhase_ = ExtrusionPhase::FIRST_TURN;
            }
            break;

        //台座を誤検知しないように旋回しておきたい
        case ExtrusionPhase::FIRST_TURN:
            if(pivotTurn_->turn(turnAngle)){
                extrusionPhase_ = ExtrusionPhase::SECOND_TURN;
            }
            break;

        //ラインまで旋回
        case ExtrusionPhase::SECOND_TURN:
            if(turn(isRightCurve,10)){
                extrusionPhase_ = ExtrusionPhase::END_LINE_TRACE;
            }
            break;

        //直角までライントレース
        case ExtrusionPhase::END_LINE_TRACE:
            startTimeMeasurement(500);
            linetrace_->run(15,endEdge,0.4);
            if(timeMeasurement_->getResult()){
                isTimeDetected = true;
            }
            if(isTimeDetected && rightAngledDetection_->getResult(4.0)){
                extrusionPhase_ = ExtrusionPhase::START_LINE_TRACE;//状態を戻しておく
                initialized = false;
                hasExecutedPhase_ = false;
                return true;
            }
            break;
        }
        return false;
    }

    //赤、青、黄、緑を検知したらtrue
    bool ETSumoNeo::hoshitoriDetection(bool saveHoshitori){
        static colorid_t nowColor = COLOR_NONE;
        nowColor = colorDetection_->getResult();
        if(nowColor == COLOR_BLUE || nowColor == COLOR_RED ||
        nowColor == COLOR_GREEN || nowColor == COLOR_YELLOW){
            //星取保存
            if(saveHoshitori){
                switch(nowColor){
                case COLOR_RED:
                    hoshitori_ = Hoshitori::RED;break;
                case COLOR_BLUE:
                    hoshitori_ = Hoshitori::BLUE; break;
                case COLOR_YELLOW:
                    hoshitori_ = Hoshitori::YELLOW;break;
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

    //星取が判明してから決まる値
    void ETSumoNeo::setValue(){
        switch(hoshitori_){
        case Hoshitori::RED:
            climbBeforeLittleAngle_ = -7;
            climbAfterSideFaceAngle_ = -85;
            isRightRotation_ = true;
            firstWrestlerColor_ = Hoshitori::BLUE;
            secondWrestlerColor_ = Hoshitori::YELLOW;
            thirdWrestlerColor_ = Hoshitori::GREEN;
            upperStageEdge_ = LineTraceEdge::LEFT;
            downStageEdge_ = LineTraceEdge::LEFT;
            firstTurnAngle_ = 90;
            secondTurnAngle_ = 90;
            thirdTurnAngle_ = -90;
            isRightCurve_ = true;
            isCorrect_ = false;
            break;

        case Hoshitori::BLUE:
            climbBeforeLittleAngle_ = 7;
            climbAfterSideFaceAngle_ = 85;
            isRightRotation_ = false;
            firstWrestlerColor_ = Hoshitori::RED;
            secondWrestlerColor_ = Hoshitori::GREEN;
            thirdWrestlerColor_ = Hoshitori::YELLOW;
            upperStageEdge_ = LineTraceEdge::RIGHT;
            downStageEdge_ = LineTraceEdge::RIGHT;
            firstTurnAngle_ = -90;
            secondTurnAngle_ = -90;
            thirdTurnAngle_ = 90;
            isRightCurve_ = false;
            isCorrect_ = true;
            break;

        case Hoshitori::YELLOW:
            climbBeforeLittleAngle_ = -7;
            climbAfterSideFaceAngle_ = -85;
            isRightRotation_ = true;
            firstWrestlerColor_ = Hoshitori::BLUE;
            secondWrestlerColor_ = Hoshitori::RED;
            thirdWrestlerColor_ = Hoshitori::GREEN;
            upperStageEdge_ = LineTraceEdge::RIGHT;
            downStageEdge_ = LineTraceEdge::LEFT;
            firstTurnAngle_ = 90;
            secondTurnAngle_ = -90;
            thirdTurnAngle_ = -90;
            isRightCurve_ = false;
            isCorrect_ = false;
            break;

        case Hoshitori::GREEN:
            climbBeforeLittleAngle_ = 7;
            climbAfterSideFaceAngle_ = 85;
            isRightRotation_ = false;
            firstWrestlerColor_ = Hoshitori::RED;
            secondWrestlerColor_ = Hoshitori::BLUE;
            thirdWrestlerColor_ = Hoshitori::YELLOW;
            upperStageEdge_ = LineTraceEdge::LEFT;
            downStageEdge_ = LineTraceEdge::RIGHT;
            firstTurnAngle_ = -90;
            secondTurnAngle_ = 90;
            thirdTurnAngle_ = 90;
            isRightCurve_ = true;
            isCorrect_ = true;
            break;

        default : break;
        }
    }

    //ラインまで旋回
    bool ETSumoNeo::turn(bool isRight, int speed){
        if(isRight){
            curveRunning_->run(-speed,speed);
        }else{
            curveRunning_->run(speed,-speed);
        }
        return lineDetection_->getResult();
    }

    //上段に向けてカーブする
    bool ETSumoNeo::curve(bool isRight, int speed){
        if(isRight){
            curveRunning_->run(0,speed);
            return bodyAngleMeasurement_->getResult() <= -90;
        }else{
            curveRunning_->run(speed,0);
            return bodyAngleMeasurement_->getResult() >= 90;
        }
    }

    //ライン復帰しやすいように補正する
    bool ETSumoNeo::correction(){
        if(isCorrect_){
            startDistanceMeasurement(70);
            straightRunning_->run(15);
            return distanceMeasurement_->getResult();
        }else{
            return true;
        }
    }

    //距離検知をまとめたもの
    void ETSumoNeo::startDistanceMeasurement(int distance){
        if(!hasExecutedPhase_){
            distanceMeasurement_->setTargetDistance(distance);
            distanceMeasurement_->startMeasurement();
            hasExecutedPhase_ = true;
        }
    }

    //時間検知をまとめたもの
    void ETSumoNeo::startTimeMeasurement(int time){
        if(!hasExecutedPhase_){
            timeMeasurement_->setBaseTime();
            timeMeasurement_->setTargetTime(time);
            hasExecutedPhase_ = true;
        }
    }

}
