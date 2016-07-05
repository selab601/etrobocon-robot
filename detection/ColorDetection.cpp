#include "ColorDetection.h"

using namespace ev3api;
namespace detection{
  ColorDetection::ColorDetection(){
    display_ = device::Display::getInstance();
    colorSensor_ = device::ColorSensor::getInstance();
  }

  colorid_t ColorDetection::getResult(){
    char message[100];
    uint8_t brightnessnumber = colorSensor_->getBrightness();
    int colornumber = colorSensor_->getColorNumber();

    switch(colorSensor_->getColorNumber()){
      case COLOR_BLACK:
        sprintf(message,"BLACK %2d",brightnessnumber);
        display_->clearDisplay();
        display_->updateDisplay(message, 5);
        return COLOR_BLACK;
        break;
      case COLOR_BLUE:
        sprintf(message,"BLUE %2d",brightnessnumber);
        display_->clearDisplay();
        display_->updateDisplay(message, 5);
        return COLOR_BLUE;
        break;
      case COLOR_RED:
      	sprintf(message,"RED %2d",brightnessnumber);
      	display_->clearDisplay();
      	display_->updateDisplay(message, 5);
        return COLOR_RED;
        
        break;
      case COLOR_GREEN:
       	sprintf(message,"GREEN %2d",brightnessnumber);
      	display_->clearDisplay();
      	display_->updateDisplay(message, 5);
        return COLOR_GREEN;
        break;
      case COLOR_YELLOW:
      	sprintf(message,"YELLOW %2d",brightnessnumber);
      	display_->clearDisplay();
      	display_->updateDisplay(message, 5);
        return COLOR_YELLOW;
        break;
      default:
        if(brightnessnumber < 10){
          sprintf(message,"BLACK %2d",brightnessnumber);
          display_->clearDisplay();
          display_->updateDisplay(message, 5);
          return COLOR_BLACK;
        }
        if(10 <= brightnessnumber && brightnessnumber <= 12){
          sprintf(message,"BLUE %2d",brightnessnumber);
          display_->clearDisplay();
          display_->updateDisplay(message, 5);
          return COLOR_BLUE;
        }
        if(13 <= brightnessnumber && brightnessnumber < 19){
          sprintf(message,"GREEN %2d",brightnessnumber);
          display_->clearDisplay();
          display_->updateDisplay(message, 5);
          return COLOR_GREEN;
        }
        else{
        	sprintf(message,"NO_COLOR %2d",brightnessnumber);
        	display_->clearDisplay();
        	display_->updateDisplay(message, 5);
          return COLOR_NONE;
        }
        break;
    }
  }
}