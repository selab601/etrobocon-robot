#ifndef GYROSENSOR_H_
#define GYROSENSOR_H_

#include "GyroSensor.h"

using namespace ev3api;

class Gyro
{
private:
    //! デバイスオブジェクト
    GyroSensor gyroSensor;

    //! 共有インスタンス
    static Gyro* _instance;

    int16_t offset;

private:
    Gyro();

public:
    static Gyro* getInstance();

    /**
     * @brief ジャイロセンサの角位置を0にリセットする．
     */
    void reset();

    /**
     * @brief 角速度に対するオフセットを設定する．(オフセット値 = 角速度0の時のセンサ値)
     * @param offset オフセット値
     */
    void setOffset(int16_t offset);

    /**
     * @brief 角速度の取得
     * @return 角速度 [deg/sec]
     */
    int16_t getAnglerVelocity();

    /**
     * @brief 角位置の取得
     * @return 角位置 [def]
     */
    int16_t getAngle();
};

#endif
