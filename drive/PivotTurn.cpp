/**
 * @brief  超信地旋回 クラス
 */

#include "PivotTurn.h"

namespace drive{
  PivotTurn::PivotTurn(){
    motor_ = device::Motors::getInstance();
    bodyAngleMeasurement_ = measurement::BodyAngleMeasurement();
    phase_ = Phase::STANDBY;
  }

  bool PivotTurn::turn(int degree_, int speed_){
    switch(phase_){
      //準備
      case Phase::STANDBY:
        bodyAngleMeasurement_.setBaceAngle();
        phase_ = Phase::IN_PROGRESS;
        break;

      //旋回
      case Phase::IN_PROGRESS:
        //絶対値を取り，旋回角度が目標角度を越えたら
        if(abs(bodyAngleMeasurement_.getResult()) >= abs(degree_)){
          phase_ = Phase::DONE;
          motor_->setWheelPWM(0, 0);
        }else{
          if(degree_ < 0){speed_ *= -1;}
          motor_->setWheelPWM(-speed_,speed_);
        }
        break;
        
      //停止
      case Phase::DONE:
        motor_->setWheelPWM(0, 0);
        //初期値にする
        phase_ = Phase::STANDBY;
        return true;
        break;
    }
    return false;
  }
}