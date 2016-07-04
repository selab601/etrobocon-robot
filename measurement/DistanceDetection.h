#ifndef _DISTANCE_DETECTION_H
#define _DISTANCE_DETECTION_H

#include "SelfPositionEstimation.h"
#include "ev3api.h"

namespace measurement{
  /**
   * @brief 距離検知クラス
   */
  class DistanceDetection {
  private:
    //! 基底距離
    long baseDistance;

    //! 目標距離
    long targetDistance;

    //! 自己位置推定
    SelfPositionEstimation* selfpositionestimation;

  public:
    /**
     * @brief コンストラクタ
     */
    DistanceDetection();

    /**
     * @brief 目標距離に達したかどうかを検知します
     * 目標距離は基底距離から進みたい長さを指します
     * @return 目標距離に到達した(true), していない(false)
     */
    bool getResult();

    /**
     * @brief 現在までの走行距離を基底距離として設定します
     */
    void startDetection();

    /**
     * @brief 目標距離を設定します
     * 目標距離は基底距離から進みたい長さを指します
     * @param 目標距離
     */
    void setTargetDistance(int distance);
  };
};

#endif
