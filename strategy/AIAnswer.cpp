#include "AIAnswer.h"

using namespace drive;
using namespace device;
using namespace detection;
using namespace measurement;
int digitalNum =0;
int analogNum =0;

namespace strategy{
    AIAnswer::AIAnswer(){
        linetrace_              = LineTrace::getInstance();
        objectDetection_        = new ObjectDetection();
        rightAngledDetection_   = new RightAngledDetection();
        timeMeasurement_        = new TimeMeasurement();
        distanceMeasurement_    = new DistanceMeasurement();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();
        straightRunning_        = new StraightRunning();
        train_                  = Train::getInstance();
        arm_                    = Arm::getInstance();

        hasExecutedPhase_       = false;
        strategySuccess_        = false;
        isLineTraceReset_       = false;
        colorDetected_          = false;

        // 黒->白の直角だけを判定する
        rightAngledDetection_->setDetectBlack(false);
        rightAngledDetection_->setDetectWhite(true);
    }

    bool AIAnswer::capture(){
        if(!strategySuccess_){
            //難所攻略手順を1つずつ実行する
            if(executeStrategy(strategyProcedure_[procedureNumber_])){
                lineTraceReset();
                distanceMeasurement_->reset();
                forcingOut_.reset();
                procedureNumber_++;
                hasExecutedPhase_ = false;//フラグを戻しておく
                isLineTraceReset_ = false;
                colorDetected_ = false;
                ev3_speaker_play_tone ( 500, 100);//音を出す
            }
        }
        if(procedureNumber_ == strategyProcedure_.size()){//最後まで終わったら
            strategySuccess_ = true;
            return true;
        }
        return strategySuccess_;
    }

    //戦略手順を実行する
    bool AIAnswer::executeStrategy(StrategyPhase strategyPhase){
        static int diffDegree10 = 0;
        switch(strategyPhase){

        //車体角度保存
        case StrategyPhase::INIT:
            bodyAngleMeasurement_->setBaseAngle();//始めの角度をセット
            return true;


        //すこしライントレース
        case StrategyPhase::LINE_TRACE_LITTLE:
            distanceMeasurement_->start(60);
            linetrace_->setPid(LineTracePid::RETURN);
            linetrace_->run(20,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();


        case StrategyPhase::LINE_RETURN:
            lineTraceReset();
            distanceMeasurement_->start(140);
            linetrace_->setTarget(0.3); // 板の色が黒に近いため黒寄りにしておく
            linetrace_->setMaxPwm(20);
            linetrace_->setEdge(LineTraceEdge::LEFT);
            linetrace_->setPid(0.09, 0, 4.3605);
            linetrace_->run();
            return distanceMeasurement_->getResult();


//AIANSWER ここから

        case StrategyPhase::TO_RIGHT_ANGLE:///直角検知
            lineTraceReset();
            linetrace_->setTarget(0.3);
            linetrace_->setMaxPwm(20);
            linetrace_->setPid(LineTracePid::VERY_FAST);
            linetrace_->run();
            return rightAngledDetection_->getResult(6.0);

        case StrategyPhase::STABILIZE:///検知後４センチ前進
            lineTraceReset();
            distanceMeasurement_->start(40);
            linetrace_->setTarget(0.3);
            linetrace_->setMaxPwm(20);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run();
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_25R:
            distanceMeasurement_->start(25);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(10,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_50R:
            distanceMeasurement_->start(50);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(10,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();


        case StrategyPhase::LINE_TRACE_100R:
            distanceMeasurement_->start(100);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(20,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_300R:
            distanceMeasurement_->start(300);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(20,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_350R:
            distanceMeasurement_->start(350);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(20,LineTraceEdge::RIGHT);
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_25L:
            distanceMeasurement_->start(25);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(10,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_50L:
            distanceMeasurement_->start(50);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(10,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();


        case StrategyPhase::LINE_TRACE_100L:
            distanceMeasurement_->start(100);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(20,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_300L:
            distanceMeasurement_->start(300);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(20,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();

        case StrategyPhase::LINE_TRACE_350L:
            distanceMeasurement_->start(350);
            linetrace_->setPid(LineTracePid::SLOW);
            linetrace_->run(20,LineTraceEdge::LEFT);
            return distanceMeasurement_->getResult();
// ９０度回転




/*
        case StrategyPhase::TO_CENTER:
            polar_.back(true);
            polar_.setMaxPwm(20);
            polar_.centerPivot(true);
            if (polar_.runTo(113, -1750 - bodyAngleMeasurement_->getRelative10()) ){
                diffDegree10 = bodyAngleMeasurement_->getRelative10();
                return true;
            }
            return false;
*/
        case StrategyPhase::TURN_LEFT_90: // 車体の角度をデジタルの方に向ける(左90度)
            polar_.back(false);
            return polar_.bodyTurn(900, 10);

         case StrategyPhase::TURN_RIGHT_90: // 車体の角度をデジタルの方に向ける(左90度)
            polar_.back(false);
            return polar_.bodyTurn(-900, 10);


  //進む 
        case StrategyPhase::FORWARD_50:
            straightRunning_->run(10);
            distanceMeasurement_->start(50);
            return distanceMeasurement_->getResult();

        case StrategyPhase::FORWARD_300:
            straightRunning_->run(20);
            distanceMeasurement_->start(300);
            return distanceMeasurement_->getResult();

                  
        case StrategyPhase::BACK_50:
            straightRunning_->run(-10);
            distanceMeasurement_->start(50);
            return distanceMeasurement_->getResult();  
                
        case StrategyPhase::BACK_100:
            straightRunning_->run(-15);
            distanceMeasurement_->start(100);
            return distanceMeasurement_->getResult();

        case StrategyPhase::BACK_150:
            straightRunning_->run(-10);
            distanceMeasurement_->start(150);
            return distanceMeasurement_->getResult();

        case StrategyPhase::BACK_300:
            straightRunning_->run(-20);
            distanceMeasurement_->start(300);
            return distanceMeasurement_->getResult();

  //おす
        case StrategyPhase::FORCING_OUT_TTT:       
            forcingOut_.setDegree(40);
        
        //押し出し
        case StrategyPhase::FORCING_OUT_DEG4:
            if(number_degital - 4 >= 0){
                if (forcingOut_.run(30, 0)){
             //   straightRunning_->run(-10);
              //  distanceMeasurement_->start(50);
                 number_degital = number_degital-4;
                 return true;
                }
            }
            else{
                if (forcingOut_.run(40, 1)){
              //  straightRunning_->run(-10);
              //  distanceMeasurement_->start(50);
                 return true;
                }
            }
            return false;

        case StrategyPhase::FORCING_OUT_DEG2:
            if(number_degital - 2 >= 0){
                if (forcingOut_.run(30, 0)){
                 number_degital = number_degital-2;
                 return true;
                }
            }
            else{
                if (forcingOut_.run(40, 1)){
                 return true;
                }
            }
            return false;

        case StrategyPhase::FORCING_OUT_DEG1:
            if(number_degital - 1 >= 0){
                if (forcingOut_.run(30, 0)){
                 number_degital = number_degital-1;
                 return true;
                }
            }
            else{
                if (forcingOut_.run(40, 1)){
                 return true;
                }
            }
            return false;

        case StrategyPhase::FORCING_OUT_ANA4:
            if(number_analog - 4 >= 0){
                if (forcingOut_.run(30, 1)){
                 number_analog = number_analog-4;
                 return true;
                }
            }
            else{
                if (forcingOut_.run(40, 0)){
                 return true;
                }
            }
            return false;
        case StrategyPhase::FORCING_OUT_ANA2:
            if(number_analog - 2 >= 0){
                if (forcingOut_.run(30, 1)){
                 number_analog = number_analog-2;
                 return true;
                }
            }
            else{
                if (forcingOut_.run(40, 0)){
                 return true;
                }
            }
            return false;

        case StrategyPhase::FORCING_OUT_ANA1:
            if(number_analog - 1 >= 0){
                if (forcingOut_.run(30, 1)){
                 number_analog = number_analog-1;
                 return true;
                }
            }
            else{
                if (forcingOut_.run(40, 0)){
                 return true;
                }
            }
            return false;
//押し出しここまで


        case StrategyPhase::TURN_LEFT_180: // 車体の角度をデジタルの方に向ける(左180度)
            polar_.back(false);
            return polar_.bodyTurn(1800, 20);


///AIANSWER ここまで

        case StrategyPhase::TURN_STRAIGHT:
            polar_.centerPivot(true);
            polar_.back(false);
            return polar_.bodyTurn(-diffDegree10, 15);

        case StrategyPhase::ARM_UP:
            return arm_->setDegree(60);

        case StrategyPhase::ARM_HOLD:
            return arm_->setDegree(35);

        case StrategyPhase::ARM_NORMAL:
            return arm_->normal();

/*
        case StrategyPhase::TO_BLOCK1:
            arm_->setDegree(35);    // 高速化のため同時にアームを上げる
            return polarToBlock(557);

        case StrategyPhase::BACK1:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800 +557 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK2:
            return polarToBlock(1243 +SLIP_DEGREE10); // タイヤが滑ってだんだん右にずれるので応急処置

        case StrategyPhase::BACK2:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800+SLIP_DEGREE10 +1243 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK3:
            return polarToBlock(2357 + SLIP_DEGREE10*2); // タイヤが滑ってだんだん右にずれるので応急処置

        case StrategyPhase::BACK3:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800 +2357+SLIP_DEGREE10*2 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_BLOCK4:
            return polarToBlock(3043 + SLIP_DEGREE10*3); // タイヤが滑ってだんだん右にずれるので応急処置で+15
*/

        case StrategyPhase::BACK4:
            polar_.back(true);
            return polar_.runTo(CENTER_TO_BLOCK_LENGTH, -1800 +3043+SLIP_DEGREE10*3 - bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_LINE_RETURN:
            arm_->normal();     // 高速化のため同時にアームを戻す
            polar_.back(true);
            return polar_.runTo(130, 1800 +SLIP_DEGREE10*3- bodyAngleMeasurement_->getRelative10());

        case StrategyPhase::TO_LEFTEDGE:
            polar_.back(false);
            return polar_.bodyTurn(150, 30);

        case StrategyPhase::FORCING_OUT:
            if (forcingOut_.run(30, isRight_)){
                blockPlaceNum_++;
                return true;
            }
            return false;

        case StrategyPhase::SEE_BLOCK:
            {
            colorid_t color = colorDetection_.getResult();
            if(!colorDetected_){
                colorDetected_ = true;
                return false;
            }
            isRight_ = (color == blockPlaceColors_[blockPlaceNum_]);
            // 奇数番号だったら寄り切りと押出しが左右逆になる
            if (blockPlaceNum_ % 2){
                isRight_ = !isRight_;
            }

            // 寄り切りのときは控えめにする
            if (color == blockPlaceColors_[blockPlaceNum_]){
                forcingOut_.setDegree(22);
            }
            else{
                forcingOut_.setDegree(35);
            }
            }
            return true;

        //検知した後すこし待つ
        case StrategyPhase::WAIT_2_SEC:
            //startTimeMeasurement(2000);
            straightRunning_->run(0);
            return timeMeasurement_->getResult();

        case StrategyPhase::NEXT_STAGE:
            polar_.setMaxPwm(50);
            polar_.back(false);
            return polar_.runTo(360, 10 - bodyAngleMeasurement_->getRelative10());

        default: return false;
        }
        return false;
    }


    void AIAnswer::lineTraceReset(){
        if(!isLineTraceReset_){
            linetrace_->reset();
            isLineTraceReset_ = true;
        }
    }


}
