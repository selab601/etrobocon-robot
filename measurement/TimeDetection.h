#ifndef _TIME_DETECTION_H
#define _TIME_DETECTION_H

#include "ev3api.h"
#include "Clock.h"

using namespace ev3api;

namespace detection{
  class TimeDetection {

  private:
    //! 時間取得
    Clock clock;

    //! 基底時間 
    uint32_t base_time_;
    //! 目標時間 
    uint32_t target_time_;


  public:
    /**
     * @brief コンストラクタ
      */
    TimeDetection();

    /**
     * @brief 目標時間に達したかどうかを検知する
     * 目標時間は基底時間からの経過時間を指す
     * @return 目標時間に達した(true)，してない(false)
      */
    bool isDetected();
    
    /**
     * @brief 基底時間をセットする
      */
    void setBaseTime();
    
    /**
     * @brief 目標時間をセットする
     * 単位はミリ秒なので10秒を目標時間にしたかったら「10000」を引数に指定してください
      */
    void setTargetTime(uint32_t target_time);
  };
};

#endif

