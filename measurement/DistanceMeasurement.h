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
    SelfPositionEstimation* selfPositionEstimation_;

    //距離検知を開始したかどうか
    bool isStartMeasurement_;

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
     * @brief startMeasurement,setTargetDistanceを同時に実行する
     * @details フラグ管理をしているので使う場合はriset（）が必要
     *
     * @param distance 目標距離
     */
    void startMeasurement(int distance);

    /**
     * @brief 目標距離を設定します
     * 目標距離は基底距離から進みたい長さを指します
     * @param 目標距離(distance > 0)
     */
    void setTargetDistance(int distance);

    /**
     * @brief 規定距離、目標距離、フラグをリセットする
     */
    void reset();

    /**
     * @brief 目標距離までの残りの距離を返す
     * @details 測定を開始していることが条件
     * @return 残りの距離,計測開始していなければ -1 を返す
     */
    long getRemainingDistance();
  };
};

#endif
