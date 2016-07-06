#ifndef _COLOR_DETECTION_H
#define _COLOR_DETECTION_H

#include "../device/ColorSensor.h"
#include "../device/Display.h"
#include <ev3api.h>

namespace detection{
  class ColorDetection{
  private:
    int colorNumber_;                //! 現在検知してる色
    rgb_raw_t rgbColor_;             //! rgb
    double greenPerRed_,bluePerRed_;
    device::ColorSensor* colorSensor_;
    device::Display* display_;

  public:
    ColorDetection();

  /**
   * @brief 検知した色を返す
   * @return 色
   */
     colorid_t getResult(); // namespace つけたほうがいいかも
  };
}
#endif
