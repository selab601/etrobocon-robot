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
        polar_.centerPivot(false);
    }

    bool ForcingOutRunning::run(int speed ,bool isRight){
        isRight_ = isRight;
        speed_   = speed;
        polar_.setMaxPwm(speed);
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
            polar_.back(false);
            if(isRight_){
                return polar_.bodyTurn(-degree_*10, speed_);
            }else{
                return polar_.bodyTurn(degree_*10, speed_);
            }

        //アームを上げる
        case Phase::ARM_UP:
            curveRunning_->run(0,0);
            return arm_->setDegree(30);

        //アームを戻す
        case Phase::ARM_DOWN:
            return arm_->normal();

        //開始地点に戻る
        case Phase::BACK:
            polar_.back(true);
            if(isRight_){
                return polar_.bodyTurn(degree_*10, speed_);
            }else{
                return polar_.bodyTurn(-degree_*10, speed_);
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

    void ForcingOutRunning::setDegree(int degree){
        degree_ = degree;
    }
}
