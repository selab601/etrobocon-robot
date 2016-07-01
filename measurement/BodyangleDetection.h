#ifndef BODYANGLE_DETECTION_
#define BODYANGLE_DETECTION_

#include "SelfPositionEstimation.h"
#include "ev3api.h"

namespace measurement{
  /**
   * 車体角度検知クラス
   */
  class BodyangleDetection{
  private:
    //! 基底角度
    int baceAngle;
    
    //! 自己位置推定
    SelfPositionEstimation* selfpositionestimation;
  
  
  public:
    /**
     * @brief コンストラクタ
     */
    BodyangleDetection();

    /**
     * @brief 現在の角度を基底角度として設定します
     */  
    void startDetection();

    /**
     * @brief 基底角度からの現在の角度を返す
     */  
    int getBodyangleDetection();
  
  };
};

#endif
