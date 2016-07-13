/**
 * @brief  超信地旋回 クラス
 */

#include "PivotTurn.h"

namespace drive{
    PivotTurn::PivotTurn(){
        motor_ = device::Motors::getInstance();
        bodyAngleMeasurement_ = measurement::BodyAngleMeasurement();
        state_ = prepare_;
    }

  bool PivotTurn::turn(int degree_){
    return turn(degree_, DEFAULT_TURN_SPEED);
  }

  bool PivotTurn::turn(int degree_, int speed_){
        switch(state_){

        //準備
        case prepare_:
          bodyAngleMeasurement_.setBaceAngle();
          state_ = isTurning_;
          break;

        //旋回
        case isTurning_:
            //目標角度まで旋回
            if(bodyAngleMeasurement_.getResult() == degree_ || bodyAngleMeasurement_.getResult() == -degree_){
              state_ = isTurned_;
              motor_->setWheelPWM(0, 0);
            }else{
              if(degree_ < 0){speed_ *= -1;}
              motor_->setWheelPWM(-speed_,speed_);
            }
            break;
        
        //停止
        case isTurned_:
          motor_->setWheelPWM(0, 0);
          //初期値にする
          state_ = prepare_;
          return true;
          break;
        }

        return false;
  }
}