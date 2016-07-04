#include "ColorDetection.h"

using namespace ev3api;
namespace detection{
  ColorDetection::ColorDetection() {
    display_ = device::Display::getInstance();
    colorSensor_ = device::ColorSensor::getInstance();
  }

  bool ColorDetection::getResult(){
    char message[100];
    uint8_t brightnessnumber = colorSensor_->getBrightness();
    int colornumber = colorSensor_->getColorNumber();

    switch(colorSensor_->getColorNumber()){
    case COLOR_BLACK:
      if(brightnessnumber < 12){
	sprintf(message,"BLACK %2d",brightnessnumber);
	display_->clearDisplay();
	display_->updateDisplay(message, 5);
      }
      break;
    case COLOR_BLUE:
      if(5 < brightnessnumber && brightnessnumber < 20){
	sprintf(message,"BLUE %2d",brightnessnumber);
	display_->clearDisplay();
	display_->updateDisplay(message, 5);
      }
      break;
    case COLOR_RED:
      if(50 < brightnessnumber && brightnessnumber < 75){
	sprintf(message,"RED %2d",brightnessnumber);
	display_->clearDisplay();
	display_->updateDisplay(message, 5);
      }
      break;
    case COLOR_GREEN:
      if(10 < brightnessnumber && brightnessnumber < 20){
	sprintf(message,"GREEN %2d",brightnessnumber);
	display_->clearDisplay();
	display_->updateDisplay(message, 5);
      }
      break;
    case COLOR_YELLOW:
      if(55 < brightnessnumber && brightnessnumber < 85){
	sprintf(message,"YELLOW %2d",brightnessnumber);
	display_->clearDisplay();
	display_->updateDisplay(message, 5);
      }
      break;
    default:
      if(30 < brightnessnumber && brightnessnumber < 45){
	sprintf(message,"GRAY %2d",brightnessnumber);
	display_->clearDisplay();
	display_->updateDisplay(message, 5);
      }
      else{
	sprintf(message,"NO_COLOR %2d",brightnessnumber);
	display_->clearDisplay();
	display_->updateDisplay(message, 5);
      }
      break;
    }

  }
}
