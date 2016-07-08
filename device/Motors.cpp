#include "Motors.h"

namespace device
{
    // インスタンス変数初期化
    Motors* Motors::instance_ = 0;

    // シングルトン
    Motors* Motors::getInstance(){
        if(instance_ == 0){
            instance_ = new Motors();
        }
        return instance_;
    }

    Motors::Motors()
        // ポートの指定
        :armMotor_(PORT_B),
         leftMotor_(PORT_D),
         rightMotor_(PORT_A),
         tailMotor_(PORT_C),
         steering_(leftMotor_, rightMotor_)
    {
        armMotor_.reset();
        leftMotor_.reset();
        rightMotor_.reset();
        tailMotor_.reset();

        preArmMotorCount_ = 0;
        preLeftMotorCount_ = 0;
        preRightMotorCount_ = 0;
        preTailMotorCount_ = 0;
    }

    void Motors::updateCount(){
        preArmMotorCount_ = armMotor_.getCount();
        preLeftMotorCount_ = leftMotor_.getCount();
        preRightMotorCount_ = rightMotor_.getCount();
        preTailMotorCount_ = tailMotor_.getCount();
    }

    void Motors::reset(){
        armMotor_.reset();
        leftMotor_.reset();
        rightMotor_.reset();
        tailMotor_.reset();
    }

    void Motors::initCount(){
        // setCount は，引数のエンコーダ値と現在のエンコーダ値の差分をオフセットとして設定する
        // getCount は，現在のエンコーダ値を返す
        // 従って，getCount によって得られたエンコーダ値を setCount の引数にとることによって．
        // エンコーダ値のオフセットを 0 に設定することができる．
        armMotor_.setCount(armMotor_.getCount());
        tailMotor_.setCount(tailMotor_.getCount());
        leftMotor_.setCount(leftMotor_.getCount());
        rightMotor_.setCount(rightMotor_.getCount());
    }

    int32_t Motors::getCount(motor_kind kind){
        int32_t count = 0;

        switch (kind){
        case MOTOR_ARM:
            count = armMotor_.getCount(); break;
        case MOTOR_LEFT:
            count = leftMotor_.getCount(); break;
        case MOTOR_RIGHT:
            count = rightMotor_.getCount(); break;
        case MOTOR_TAIL:
            count = tailMotor_.getCount(); break;
        default:
            // TODO: きちんと例外処理する
            throw;
        }

        return count;
    }

    int32_t Motors::getPreCount(motor_kind kind){
        int32_t preCount = 0;

        switch (kind){
        case MOTOR_ARM:
            preCount = preArmMotorCount_; break;
        case MOTOR_LEFT:
            preCount = preLeftMotorCount_; break;
        case MOTOR_RIGHT:
            preCount = preRightMotorCount_; break;
        case MOTOR_TAIL:
            preCount = preTailMotorCount_; break;
        default:
            // TODO: きちんと例外処理する
            throw;
        }

        return preCount;
    }

    void Motors::setPWM(motor_kind kind, int pwm){
        switch (kind){
        case MOTOR_ARM:
            armMotor_.setPWM(pwm); break;
        case MOTOR_LEFT:
            leftMotor_.setPWM(pwm); break;
        case MOTOR_RIGHT:
            rightMotor_.setPWM(pwm); break;
        case MOTOR_TAIL:
            tailMotor_.setPWM(pwm); break;
        default:
            // TODO: きちんと例外処理する
            throw;
        }
    }

    void Motors::setWheelPWM(int leftPWM, int rightPWM){
        leftMotor_.setPWM(leftPWM);
        rightMotor_.setPWM(rightPWM);
    }

    void Motors::steerWheel(int power, int turnRatio){
        steering_.setPower(power, turnRatio);
    }
}
