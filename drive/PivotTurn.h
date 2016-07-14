/**
 * @brief  超信地旋回　クラス 
 */

#ifndef _PIVOT_TURN_H
#define _PIVOT_TURN_H

#include "../device/Motors.h"
#include "../measurement/BodyAngleMeasurement.h"
#include "cstdlib" //abs() for integer

#define DEFAULT_TURN_SPEED 20

namespace drive
{
  class PivotTurn{
    public:
      /**
       * @brief コンストラクタ
       * @details [long description]
       */
      PivotTurn();

      /**
       * @brief degree度回転させる 左が正，右が負
       * @details 左方向に90度回転させるとき:turn(90)
       * @param degree 回転させる角度
       * @param speed  回転させる速さ(0~100, デフォルト:20)
       * @return 終了した場合true
       */
      bool turn(int degree_, int speed_ = DEFAULT_TURN_SPEED);

    private:
      device::Motors* motor_;
      measurement::BodyAngleMeasurement bodyAngleMeasurement_;
        
      enum class Phase
      {
        STANDBY,
        IN_PROGRESS,
        DONE
      };
      Phase phase_;
  };
}
#endif
