#ifndef _TIME_MEASUREMENT_H
#define _TIME_MEASUREMENT_H

#include "ev3api.h"
#include "Clock.h"

namespace measurement{
  class TimeMeasurement {

  private:
    //! 時間取得
    ev3api::Clock clock;

    //! 基底時間
    uint32_t baseTime_;
    //! 目標時間
    uint32_t targetTime_;


  public:
    /**
     * @brief コンストラクタ
      */
    TimeMeasurement();

    /**
     * @brief 目標時間に達したかどうかを検知する
     * 目標時間は基底時間からの経過時間を指す
     * @return 目標時間に達した(true)，してない(false)
      */
    bool getResult();

    /**
     * @brief 基底時間をセットする
      */
    void setBaseTime();

    /**
     * @brief 目標時間をセットする
     * 単位はミリ秒なので10秒を目標時間にしたかったら「10000」を引数に指定してください
      */
    void setTargetTime(uint32_t targetTime);
  };
};

#endif
