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
		static int8_t whiteValue = 0;			//白の値
		static int8_t blackValue = 0;			//黒の値
		static int8_t colorValue = 0;			//キャリブレーションの値
		static bool isCalibratedWhite = false;	// 白の値のキャリブレーションが終わっているか
		static bool isCalibratedBlack = false;	// 黒の値のキャリブレーションが終わっているか
		static bool isConfirmed = false;		//  キャリブレーションの結果の確認が終わっているか

		// タッチセンサの押下状態を保持する静的変数,押下されたらtrue,離れたらfalseにする
		static bool hasPressed = true;

		// キャリブレーションが終了し、結果確認が終わっているか
		bool calibrated = isCalibratedWhite && isCalibratedBlack && isConfirmed;

		char message[100];

		//　タッチセンサの押下状態の更新(長押し中に処理が進まないようにするため)
		if ( hasPressed && touch->isPressed() ){
			return calibrated;
		}
		else{
			hasPressed = false;
		}

		// カラーセンサ（白）のキャリブレーション
		if ( !isCalibratedWhite ){
			calibrated = false;
			colorValue = colorCalibrate(WHITE);
			if( colorValue != -1 ){
				hasPressed = true;
				whiteValue = colorValue;
				isCalibratedWhite = true;

			}
		}
		// カラーセンサ（黒）のキャリブレーション
		else if ( !isCalibratedBlack ){
			calibrated = false;
			colorValue = colorCalibrate(BLACK);
			if( colorValue != -1 ){
				hasPressed = true;
				blackValue = colorValue;
				isCalibratedBlack = true;
				//キャリブレーションの値を保存
				brightnessInfo->setCalibrateValue(whiteValue, blackValue);
			}

		}
		// キャリブレーションの確認
		else if ( !isConfirmed ){
			calibrated = false;
			display-> updateDisplay("      - CALIBRATION -      ", 0);
			display-> updateDisplay("CALIBRATION FINISHED        ", 1);

			sprintf( message, "Color (W,B),C: (%d, %d), %d ",
				brightnessInfo->getWhiteCalibratedValue(),
				brightnessInfo->getBlackCalibratedValue(),
				brightnessInfo->getBrightness() );
			display-> updateDisplay(message, 2);
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

		// タッチセンサの押下状態を保持する静的変数,押下されたらtrue,離れたらfalseにする
		static bool hasPressed = true;

		//　タッチセンサの押下状態の更新(長押し中に処理が進まないようにするため)
		if ( hasPressed &&
			(touch->isPressed() || ev3_button_is_pressed (UP_BUTTON) 
				|| ev3_button_is_pressed (DOWN_BUTTON) || ev3_button_is_pressed (ENTER_BUTTON)) ){
			return courseSelected && confirmed;
		}
		else // タッチセンサが離されたとき
		{
			hasPressed = false;
		}

		// ↑ ボタンが押されたとき
		if ( ev3_button_is_pressed (UP_BUTTON) ){
			hasPressed = true;
			index--;
		}
		// ↓ ボタンが押されたとき
		else if ( ev3_button_is_pressed (DOWN_BUTTON)){
			hasPressed = true;
			index++;
		}
		// エンターボタンが押されたとき
		else if ( ev3_button_is_pressed (ENTER_BUTTON)){
			hasPressed = true;
			courseSelected = true;
			switch( index ){
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

		//indexが範囲外の場合
		if ( index < 0 ){
			index = 0;
		}
		if ( index >= COURSES_NUM ){
			index = COURSES_NUM - 1;
		}

		char courseNames[COURSES_NUM][30] = {"   Course L                ","   Course R                "};
		courseNames[index][1] = '>';

		//コースが選択されていないとき
		if ( !courseSelected ){
			display-> updateDisplay("                            ", 0);
			display-> updateDisplay("        SELECT COURSE       ", 1);
			display-> updateDisplay(courseNames[0], 2);
			display-> updateDisplay(courseNames[1], 3);
		}
		//コースが選択された時
		else if ( courseSelected && !confirmed ){
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

		// タッチセンサの押下状態を保持する静的変数,押下されたらtrue,離れたらfalseにする
		static bool hasPressed = true;
		if  ( !touch->isPressed() ){
			hasPressed = false;
		}
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
	//カラーのキャリブレーション
	int8_t StartUp::colorCalibrate(calibrationColor color){
		int8_t tmpValue;
		char calibrationColorMessage[100];
		char currentValueMessage[100];
		char colorStr[6];
		switch(color){
			case WHITE:
				strcpy(colorStr,"WHITE"); break;
			case BLACK:
				strcpy(colorStr,"BLACK"); break;
		}

		display-> updateDisplay("      - CALIBRATION -      ",  0);
		sprintf( calibrationColorMessage, "Color sensor [%s]       ",colorStr);
		display->updateDisplay(calibrationColorMessage,1);
		tmpValue = brightnessInfo->getBrightness();
		sprintf( currentValueMessage, "current value: %2d          ", tmpValue);
		display-> updateDisplay(currentValueMessage, 2);

		if ( touch->isPressed() ){
			// 音を出す
			ev3_speaker_play_tone ( 500, 100);
			return tmpValue;
		}

		return -1;
	}

}