#include "ForcingOutRunning.h"

using namespace drive;
using namespace device;
using namespace measurement;

namespace drive{
    ForcingOutRunning::ForcingOutRunning(){
        curveRunning_           = new CurveRunning();
        pivotTurn_              = new PivotTurn();
        arm_                    = Arm::getInstance();
        bodyAngleMeasurement_   = new BodyAngleMeasurement();

        isSuccess_       = false;
        isRight_         = false;
        procedureNumber_ = 0;
        speed_           = 0;
    }

    bool ForcingOutRunning::run(int speed ,bool isRight){
        isRight_ = isRight;
        speed_   = speed;
        if(!isSuccess_){
            //手順を1つずつ実行する
            if(executePhase(phaseProcedure_[procedureNumber_] ) ){
                procedureNumber_++;
            }
        }
        //最後の手順まで
        if(procedureNumber_ == phaseProcedure_.size()){
            isSuccess_ = true;
            return true;
        }
        return isSuccess_;
    }

    bool ForcingOutRunning::executePhase(Phase phase){
        switch(phase){

        //車体角度保存
        case Phase::SET_ANGLE:
            bodyAngleMeasurement_->setBaseAngle();
            return true;

        //信地旋回
        case Phase::CURVE:
            if(isRight_){
                curveRunning_->run(0,speed_);
                return bodyAngleMeasurement_->getResult() <= -60;
            }else{
                curveRunning_->run(speed_,0);
                return bodyAngleMeasurement_->getResult() >= 60;
            }

        //アームを上げる
        case Phase::ARM_UP:
            curveRunning_->run(0,0);
            return arm_->setDegree(70);

        //アームを戻す
        case Phase::ARM_DOWN:
            return arm_->normal();

        //開始地点に戻る
        case Phase::BACK:
            if(isRight_){
                curveRunning_->run(0,-speed_);
                return bodyAngleMeasurement_->getResult() >= 60;
            }else{
                curveRunning_->run(-speed_,0);
                return bodyAngleMeasurement_->getResult() <= -60;
            }

        default: return false;
        }
    }

    void ForcingOutRunning::reset(){
        isSuccess_ = false;
        isRight_ = false;
        procedureNumber_ = 0;
        speed_ = 0;
    }
}
