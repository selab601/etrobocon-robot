#include "ClimbingRunning.h"

using namespace measurement;
using namespace device;

namespace drive{

    //コンストラクタ
    ClimbingRunning::ClimbingRunning(){
        motor_ = Motors::getInstance();
        straightRunning_ = new StraightRunning();
        distanceMeasurement_ = new DistanceMeasurement();
        //countMeasurement_ = new CountMeasurement(ARM,armTargetCount);
        runningState_ = INIT;
        armBaseCount_ = 0;
        armTargetCount_ = 50;
    }


    bool ClimbingRunning::run(int speed , int distance){
        switch(runningState_){
        //初期状態
        case INIT:
            //countMeasurement_->setBaseCount();
            armBaseCount_ = motor_->getCount(MOTOR_ARM);
            runningState_ = ARM_UP;
            break;

        //アームを上げる
        case ARM_UP:

            motor_->setPWM(MOTOR_ARM,10);
            //straightRunning_->run(0);
            //if(countMeasurement_->getResult()){}
            if(armTargetCount_ + armBaseCount_ == motor_->getCount(MOTOR_ARM)){
                distanceMeasurement_->setTargetDistance(distance);
                distanceMeasurement_->startMeasurement();
                runningState_ = CLIMB;
            }
            break;

        //登壇する
        case CLIMB:
            motor_->setPWM(MOTOR_ARM,0);
            straightRunning_->run(speed);
            if(distanceMeasurement_->getResult()){
                //countMeasurement_->setBaseCount();
                //countMeasurement_->setTargetCount(-armTargetCount_);
                armBaseCount_ = motor_->getCount(MOTOR_ARM);
                runningState_ = ARM_DOWN;
            }
            break;

        //アームを下げる
        case ARM_DOWN:
            straightRunning_->run(0);
            motor_->setPWM(MOTOR_ARM,-10);
            //if(countMeasurement_->getResult()){}
            if(-armTargetCount_ + armBaseCount_ == motor_->getCount(MOTOR_ARM)){
                motor_->setPWM(MOTOR_ARM,0);
                runningState_ = END;
                //return true;
            }
            break;

        case END:
            motor_->setPWM(MOTOR_ARM,0);
            return true;
            break;
        }
        return false;
    }
}
