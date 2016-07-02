#include "ColorDetection.h"

using namespace ev3api;
namespace detection{
	ColorDetection::ColorDetection() {
		display_ = device::Display::getInstance();
		colorSensor_ = device::ColorSensor::getInstance();
	}
	

	bool ColorDetection::isDetected(){
		int colornumber = colorSensor_->getColorNumber();
		switch(colorSensor_->getColorNumber()){
			case COLOR_BLACK:
			display_->clearDisplay();
			display_->updateDisplay("BLACK", 5);
			break;
			case COLOR_BLUE:
			display_->clearDisplay();
			display_->updateDisplay("BLUE", 5);
			break;
			case COLOR_RED:
			display_->clearDisplay();
			display_->updateDisplay("RED", 5);
			break;
			case COLOR_GREEN:
			display_->clearDisplay();
			display_->updateDisplay("FREEN", 5);
			break;
			case COLOR_YELLOW:
			display_->clearDisplay();
			display_->updateDisplay("YELLOW", 5);
			break;
			default:
			display_->clearDisplay();
			display_->updateDisplay("NO_COLOR", 5);
		}

	}
}








