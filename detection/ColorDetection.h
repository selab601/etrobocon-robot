#ifndef _COLOR_DETECTION_H
#define _COLOR_DETECTION_H

#include "../device/ColorSensor.h"
#include <ev3api.h>

namespace detection{
  class ColorDetection{
  private:
    rgb_raw_t rgbColor_;             //! rgb値
    double greenPerRed_,bluePerRed_; //! g値/r値, b値/r値 
    device::ColorSensor* colorSensor_;
  public:
    ColorDetection();

  /**
   * @brief 検知した色を返す
   * @return 色
   */
     colorid_t getResult();
  };
}
#endif
