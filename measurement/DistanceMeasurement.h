#ifndef _DISTANCE_MEASUREMENT_H
#define _DISTANCE_MEASUREMENT_H

#include "SelfPositionEstimation.h"
#include "ev3api.h"

namespace measurement{
  /**
   * @brief 距離検知クラス
   */
  class DistanceMeasurement {
  private:
    //! 基底距離
    long baseDistance_;

    //! 目標距離
    long targetDistance_;

    //! 自己位置推定
    SelfPositionEstimation* selfpositionestimation_;

  public:
    /**
     * @brief コンストラクタ
     */
    DistanceMeasurement();

    /**
     * @brief 目標距離に達したかどうかを検知します
     * 目標距離は基底距離から進みたい長さを指します
     * @return 目標距離に到達した(true), していない(false)
     */
    bool getResult();

    /**
     * @brief 現在までの走行距離を基底距離として設定します
     */
    void startMeasurement();

    /**
     * @brief 目標距離を設定します
     * 目標距離は基底距離から進みたい長さを指します
     * @param 目標距離
     */
    void setTargetDistance(int distance);
  };
};

#endif
