#ifndef _COLOR_DETECTION_H
#define _COLOR_DETECTION_H

#include "../device/ColorSensor.h"
#include "../device/Display.h"
#include <ev3api.h>

namespace detection{
  class ColorDetection{
  private:
    int colornumber;
    int brightnessnumber;
    char message;

    device::ColorSensor* colorSensor_;
    device::Display* display_;

  public:
    ColorDetection();

    bool getResult();
  };
}
#endif
