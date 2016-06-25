#ifndef MOTORS_H_
#define MOTORS_H_

#include "ev3api.h"
#include "/home/vagrant/hrp2/sdk/common/library/libcpp-ev3/include/Motor.h"
#include "Steering.h"

namespace device
{
    using namespace ev3api;

    enum motor_kind {
        MOTOR_ARM,
        MOTOR_LEFT,
        MOTOR_RIGHT,
        MOTOR_TAIL
    };

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

        /**
         * @brief 種類に応じたモータオブジェクトを取得する
         * @throw 予期しないモータ種類が与えられた際のエラー
         */
        Motor getObject(motor_kind kind);

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
         * @brief 全モータのエンコーダ値のオフセットを初期化する．
         */
        void initCount();

        /**
         * @brief 各モータのエンコーダ値取得
         * @param kind モータ種類
         * @throw 予期しないモータ種類が与えられた際のエラー
         * @return エンコーダ値
         */
        int32_t getCount(motor_kind kind);

        /**
         * @brief 各モータの更新前エンコーダ値取得
         * @param kind モータ種類
         * @throw 予期しないモータ種類が与えられた際のエラー
         * @return 更新前のエンコーダ値
         */
        int32_t getPreCount(motor_kind kind);

        /**
         * @brief 各モータへのPWM値のセット
         * @param kind モータ種類
         * @param pwm 設定するPWM値
         * @throw 予期しないモータ種類が与えられた際のエラー
         */
        void setPWM(motor_kind kind, int pwm);

        void setWheelPWM(int leftPWM, int rightPWM);

        /**
         * @brief ステアリング操作を行う
         * @param power モータのパワー [-100..+100]．マイナスの値は後退
         * @param turnRatio ステアリングの度合い [-100..+100]．マイナスは左，プラスは右への転回
         */
        void steerWheel(int power, int turnRatio);
    };
}

#endif
