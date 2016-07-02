#ifndef _COLOR_DETECTION_H
#define _COLOR_DETECTION_H

#include "../device/ColorSensor.h"
#include "../device/display.h"
#include <ev3api.h>


namespace detection{
  class ColorDetection{
  private:

  	int colornumber;
    
	//enum color_detection_;

    device::ColorSensor* colorSensor_;
    device::Display* display_;

  public:
    ColorDetection();

    bool isDetected();

};

}
#endif