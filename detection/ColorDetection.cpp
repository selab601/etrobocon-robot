#include "ColorDetection.h"

using namespace device;
namespace detection{
  ColorDetection::ColorDetection(){
    display_ = Display::getInstance();
    colorSensor_ = ColorSensor::getInstance();
  }

  colorid_t ColorDetection::getResult(){
    colorSensor_->getRawColor(rgbColor_);
    greenPerRed_ = rgbColor_.g*(1.0)/(rgbColor_.r + 0.01);  //! r値とg値の比
    bluePerRed_ = rgbColor_.b*(1.0)/(rgbColor_.r + 0.01);   //! r値とb値の比

    switch(colorSensor_->getColorNumber()){                 //! 現在の色を取得
      case COLOR_BLACK:
      //実測値による比較
        if(0.6<greenPerRed_ && greenPerRed_<1.6 && 0.15<bluePerRed_&&bluePerRed_<1.1){ 
          return COLOR_BLACK;
        }
        break;
      case COLOR_BLUE:
        if(1.3<greenPerRed_ && greenPerRed_<2.9 && 1.4<bluePerRed_ && bluePerRed_<3.1){
          return COLOR_BLUE;
        }
        break;
      case COLOR_RED:
        if(0.13<greenPerRed_ && greenPerRed_<0.5 && 0.03<bluePerRed_ && bluePerRed_<2.3){
          return COLOR_RED;
        }
        break;
      case COLOR_GREEN:
        if(1.4<greenPerRed_ && greenPerRed_<3.6 && 0.3<bluePerRed_ && bluePerRed_<1.0){
          return COLOR_GREEN;
        }
        break;
      case COLOR_YELLOW:
        if(0.4<greenPerRed_ && greenPerRed_<1.2 && 0.03<bluePerRed_ && bluePerRed_<0.5){
          return COLOR_YELLOW;
        }
        default:
          break;
      }
        return COLOR_NONE;
    }
}
