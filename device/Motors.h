#ifndef MOTORS_H_
#define MOTORS_H_

#include "ev3api.h"
#include "Motor.h"
#include "Steering.h"

using namespace ev3api;

class Motors
{
private:
    // 各モータのデバイスオブジェクト
    Motor armMotor;
    Motor leftMotor;
    Motor rightMotor;
    Motor tailMotor;

    // ステアリング用デバイスオブジェクト
    Steering steering;

    //! 共有インスタンス
    static Motors* _instance;

    //! 前回のアームモータエンコーダ値
    int32_t preArmMotorCount;
    //! 前回の左輪モータエンコーダ値
    int32_t preLeftMotorCount;
    //! 前回の右輪モータエンコーダ値
    int32_t preRightMotorCount;
    //! 前回の尻尾モータエンコーダ値
    int32_t preTailMotorCount;

private:
    Motors();

public:
    static Motors* getInstance();

    /**
     * @brief 前回のエンコーダ値更新
     */
    void updateCount();

    /**
     * @brief 全モータのリセット．モータ停止及び角位置を0にリセットする．
     */
    void reset();

    /**
     * @brief 左右の車輪のモータエンコーダ値のオフセットを初期化する．
     */
    void initWheelCount();

    /**
     * @brief 全モータのエンコーダ値のオフセットを初期化する．
     */
    void initAllCount();

    // エンコーダ値取得
    int32_t getArmCount();
    int32_t getLeftCount();
    int32_t getRightCount();
    int32_t getTailCount();

    // 更新前エンコーダ値取得
    int32_t getPreArmCount();
    int32_t getPreLeftCount();
    int32_t getPreRightCount();
    int32_t getPreTailCount();

    // PWMセット
    void setArmPWM(int pwm);
    void setLeftPWM(int pwm);
    void setRightPWM(int pwm);
    void setTailPWM(int pwm);
    void setWheelPWM(int leftPWM, int rightPWM);

    /**
     * @brief ステアリング操作を行う
     * @param power モータのパワー [-100..+100]．マイナスの値は後退
     * @param turnRatio ステアリングの度合い [-100..+100]．マイナスは左，プラスは右への転回
     */
    void steerWheel(int power, int turnRatio);
};

#endif
