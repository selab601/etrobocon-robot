#include "Motors.h"

namespace device
{
    // インスタンス変数初期化
    Motors* Motors::_instance = 0;

    // シングルトン
    Motors* Motors::getInstance(){
        if(_instance == 0){
            _instance = new Motors();
        }
        return _instance;
    }

    Motors::Motors()
        // ポートの指定
        :armMotor(PORT_B),
         leftMotor(PORT_D),
         rightMotor(PORT_A),
         tailMotor(PORT_C),
         steering(leftMotor, rightMotor)
    {
        armMotor.reset();
        leftMotor.reset();
        rightMotor.reset();
        tailMotor.reset();

        preArmMotorCount = 0;
        preLeftMotorCount = 0;
        preRightMotorCount = 0;
        preTailMotorCount = 0;
    }

    Motor Motors::getObject(motor_kind kind){
        switch (kind){
        case MOTOR_ARM:
            return armMotor; break;
        case MOTOR_LEFT:
            return leftMotor; break;
        case MOTOR_RIGHT:
            return rightMotor; break;
        case MOTOR_TAIL:
            return tailMotor; break;
        }

        // TODO: きちんと例外処理する
        throw;
    }

    void Motors::updateCount(){
        preArmMotorCount = armMotor.getCount();
        preLeftMotorCount = leftMotor.getCount();
        preRightMotorCount = rightMotor.getCount();
        preTailMotorCount = tailMotor.getCount();
    }

    void Motors::reset(){
        armMotor.reset();
        leftMotor.reset();
        rightMotor.reset();
        tailMotor.reset();
    }

    void Motors::initCount(){
        // setCount は，引数のエンコーダ値と現在のエンコーダ値の差分をオフセットとして設定する
        // getCount は，現在のエンコーダ値を返す
        // 従って，getCount によって得られたエンコーダ値を setCount の引数にとることによって．
        // エンコーダ値のオフセットを 0 に設定することができる．
        armMotor.setCount(armMotor.getCount());
        tailMotor.setCount(tailMotor.getCount());
        leftMotor.setCount(leftMotor.getCount());
        rightMotor.setCount(rightMotor.getCount());
    }

    int32_t Motors::getCount(motor_kind kind){
        return Motors::getObject(kind).getCount();
    }

    int32_t Motors::getPreCount(motor_kind kind){
        int32_t preCount = -1;

        switch (kind){
        case MOTOR_ARM:
            preCount = preArmMotorCount; break;
        case MOTOR_LEFT:
            preCount = preLeftMotorCount; break;
        case MOTOR_RIGHT:
            preCount = preRightMotorCount; break;
        case MOTOR_TAIL:
            preCount = preTailMotorCount; break;
        }

        if (preCount != -1) {
            return preCount;
        } else {
            // TODO: きちんと例外処理する
            throw;
        }
    }

    void Motors::setPWM(motor_kind kind, int pwm){
        Motors::getObject(kind).setPWM(pwm);
    }

    void Motors::setWheelPWM(int leftPWM, int rightPWM){
        leftMotor.setPWM(leftPWM);
        rightMotor.setPWM(rightPWM);
    }

    void Motors::steerWheel(int power, int turnRatio){
        steering.setPower(power, turnRatio);
    }
}
