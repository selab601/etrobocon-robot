#include "Motors.h"

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

void Motors::initAllCount(){
    // setCount は，引数のエンコーダ値と現在のエンコーダ値の差分をオフセットとして設定する
    // getCount は，現在のエンコーダ値を返す
    // 従って，getCount によって得られたエンコーダ値を setCount の引数にとることによって．
    // エンコーダ値のオフセットを 0 に設定することができる．
    armMotor.setCount(armMotor.getCount());
    tailMotor.setCount(tailMotor.getCount());
    Motors::initWheelCount();
}

void Motors::initWheelCount(){
    leftMotor.setCount(leftMotor.getCount());
    rightMotor.setCount(rightMotor.getCount());
}

int32_t Motors::getArmCount(){
    return armMotor.getCount();
}

int32_t Motors::getLeftCount(){
    return leftMotor.getCount();
}

int32_t Motors::getRightCount(){
    return rightMotor.getCount();
}

int32_t Motors::getTailCount(){
    return tailMotor.getCount();
}

int32_t Motors::getPreArmCount(){
    return preArmMotorCount;
}

int32_t Motors::getPreLeftCount(){
    return preLeftMotorCount;
}

int32_t Motors::getPreRightCount(){
    return preRightMotorCount;
}

int32_t Motors::getPreTailCount(){
    return preTailMotorCount;
}

void Motors::setArmPWM(int pwm){
    armMotor.setPWM(pwm);
}

void Motors::setLeftPWM(int pwm){
    leftMotor.setPWM(pwm);
}

void Motors::setRightPWM(int pwm){
    rightMotor.setPWM(pwm);
}

void Motors::setTailPWM(int pwm){
    tailMotor.setPWM(pwm);
}

void Motors::setWheelPWM(int leftPWM, int rightPWM){
    leftMotor.setPWM(leftPWM);
    rightMotor.setPWM(rightPWM);
}

void Motors::steerWheel(int power, int turnRatio){
    steering.setPower(power, turnRatio);
}
