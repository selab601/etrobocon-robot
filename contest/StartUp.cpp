/**
 * @file StartUp.cpp
 * @note  スタートアップクラス
 * @author usui kakeru
 */
#include "StartUp.h"
#include "stdio.h"

#define COURSES_NUM 	2 // コースの数

using namespace device;

namespace contest_pkg{

StartUp* StartUp::instance = 0;

StartUp::StartUp(){
    brightnessInfo = device::ColorSensor::getInstance();
    touch = device::TouchSensor::getInstance();
    display = device::Display::getInstance();
}

StartUp* StartUp::getInstance(){
	if (!instance) {
        instance = new StartUp();
    }
    return instance;
}

bool StartUp::isFinished(){
	return selectCourse() && calibrate();
}

char StartUp::getSelectedCourse(){
	return selectedCourse;
}

bool StartUp::calibrate(){
	//キャリブレーションの状態
	static int8_t whiteValue = 0;
	static int8_t blackValue = 0;
	static bool isCalibratedWhite = false; // 白の値のキャリブレーションが終わっているか
	static bool isCalibratedBlack = false;	// 黒の値のキャリブレーションが終わっているか
	static bool isCalibratedGyro = false;	// ジャイロのキャリブレーションが終わっているか
	static bool isConfirmed = false; 		//  キャリブレーションの結果の確認が終わっているか

	// 前回にタッチセンサが押されていたらtrue
	static bool hasPressed = true;

	bool calibrated = isCalibratedWhite
				&& isCalibratedBlack
				&& isCalibratedGyro
				&& isConfirmed; //すべてのキャリブレーションが終わっていたらtrue

	char message[100];

	//　前回にタッチセンサが押されていて、まだ離されていないとき
	if ( hasPressed && touch->isPressed() )
		return calibrated;
	else // タッチセンサが離されたとき
		hasPressed = false;

	// カラーセンサ（白）のキャリブレーション
	if ( !isCalibratedWhite){
		calibrated = false;
		display-> updateDisplay("      - CALIBRATION -      ",  0);
		display-> updateDisplay("Color sensor [WHITE]        ",  1);
		int16_t tmpValue = brightnessInfo->getBrightness();
		sprintf( message, "current value: %2d          ", tmpValue);
		display-> updateDisplay(message,  2);
		display-> updateDisplay("                            ",  3);


		if ( touch->isPressed() ){
			hasPressed = true;
			whiteValue = tmpValue;
			isCalibratedWhite = true;

			// 音を出す
			ev3_speaker_play_tone ( 500, 100);
		}
	}
	// カラーセンサ（黒）のキャリブレーション
	else if ( !isCalibratedBlack){
		calibrated = false;
		display-> updateDisplay("      - CALIBRATION -      ",  0);
		display-> updateDisplay("Color sensor [BLACK]       ",  1);
		int16_t tmpValue = brightnessInfo->getBrightness();
		sprintf( message, "current value: %2d          ", tmpValue);
		display-> updateDisplay(message, 2);

		if ( touch->isPressed() ){
			hasPressed = true;
			blackValue = tmpValue;
			isCalibratedBlack = true;

			// キャリブレーション値をセットする
			brightnessInfo->setCalibrateValue(whiteValue, blackValue);

			// 音を出す
			ev3_speaker_play_tone ( 500, 100);
		}
	}
	// ジャイロセンサのキャリブレーション
	else if (!isCalibratedGyro) {
		calibrated = false;
		display-> updateDisplay("      - CALIBRATION -      ",  0);
		display-> updateDisplay("Gyro sensor              ", 1);
		// int16_t tmpValue = gyro->getGyroValue();
		// sprintf( message, "current value: %d          ", tmpValue);
		display-> updateDisplay(message, 2);

		if ( touch->isPressed() ){
			hasPressed = true;

			// キャリブレーション値をセットする（角速度0のときの値）
			// gyro->setGyroOffset(tmpValue);
			// ジャイロセンサの角位置0をリセットする
			// gyro->gyroReset();

			isCalibratedGyro = true;

			// 音を出す
			ev3_speaker_play_tone ( 500, 100);
		}
	}
	// キャリブレーションの確認
	else if (!isConfirmed){
		calibrated = false;
		display-> updateDisplay("      - CALIBRATION -      ", 0);
		display-> updateDisplay("CALIBRATION FINISHED        ", 1);

		sprintf( message, "Color (W,B),C: (%d, %d), %d ",
			brightnessInfo->getWhiteCalibratedValue(),
			brightnessInfo->getBlackCalibratedValue(),
			brightnessInfo->getBrightness() );
		display-> updateDisplay(message, 2);
		// sprintf( message, "Gyro (current value): %d   ", gyro->getGyroValue() );
		display-> updateDisplay(message, 3);
		if ( touch->isPressed() ){
			hasPressed = true;
			isConfirmed = true;
		}
	}
	else {
		calibrated = true;
	}

	return calibrated;
}

bool StartUp::selectCourse(){
	static int index = 0;
	static bool courseSelected = false;
	static bool confirmed = false;

	// 前回にタッチセンサが押されていたらtrue
	static bool hasPressed = true;

	//　前回にタッチセンサが押されていて、まだ離されていないとき
	if ( hasPressed &&
		(touch->isPressed() || ev3_button_is_pressed (UP_BUTTON) || ev3_button_is_pressed (DOWN_BUTTON) || ev3_button_is_pressed (ENTER_BUTTON)) )
		return courseSelected && confirmed;
	else // タッチセンサが離されたとき
		hasPressed = false;


		// ↑ ボタンが押されたとき
		if (ev3_button_is_pressed (UP_BUTTON) ){
			hasPressed = true;
			index--;
		}
		// ↓ ボタンが押されたとき
		else if (ev3_button_is_pressed (DOWN_BUTTON)){
			hasPressed = true;
			index++;
		}
		// エンターボタンが押されたとき
		else if (ev3_button_is_pressed (ENTER_BUTTON)){
			hasPressed = true;
			courseSelected = true;
			switch(index){
			case 0:
				selectedCourse = 'L';
				break;
			case 1:
				selectedCourse = 'R';
				break;
			}
			// 音を出す
			ev3_speaker_play_tone ( 500, 100);
		}
		if (index < 0)
			index = 0;
		if (index >= COURSES_NUM)
			index = COURSES_NUM - 1;

		char courseNames[COURSES_NUM][30] = {"   Course L                ",
									"   Course R                "};
		courseNames[index][1] = '>';

		if (!courseSelected){
			display-> updateDisplay("                            ", 0);
			display-> updateDisplay("        SELECT COURSE       ", 1);
			display-> updateDisplay(courseNames[0], 2);
			display-> updateDisplay(courseNames[1], 3);
		}
		else if (courseSelected && !confirmed){
			if (ev3_button_is_pressed (ENTER_BUTTON) || touch->isPressed() ){
				hasPressed = true;
				confirmed = true;
			}

			char message[30];
			sprintf(message, "  SELECTED COURSE: %c", selectedCourse);
			display-> updateDisplay("                            ", 0);
			display-> updateDisplay("                            ", 1);
			display-> updateDisplay("                            ", 2);
			display-> updateDisplay(message, 3);
		}

	return courseSelected && confirmed;
}
//スタートを受け入れる
bool StartUp::acceptStart(){
	    static bool started = false;
    static bool hasPressed = true;
    if  ( !touch->isPressed() )
        hasPressed = false;

    if ( !started && !hasPressed && touch->isPressed()){
        started = true;

        display-> updateDisplay ("                            ", 0);
        display-> updateDisplay ("                            ", 1);
        display-> updateDisplay ("                            ", 2);
        display-> updateDisplay ("         S T A R T          ", 3);
    }
    else if ( !started ){
        display-> updateDisplay ("                            ", 0);
        display-> updateDisplay ("                            ", 1);
        display-> updateDisplay ("                            ", 2);
        display-> updateDisplay ("         R E A D Y          ", 3);
    }
    return started;
}

}
