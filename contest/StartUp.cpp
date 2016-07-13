#include "StartUp.h"
#include "stdio.h"
#include "../measurement/DistanceMeasurement.h"
#include <Clock.h>

#define COURSES_NUM 	2 // コースの数

using namespace drive;
using namespace measurement;
using namespace ev3api;

namespace contest_pkg{

    StartUp* StartUp::instance_ = 0;

    StartUp::StartUp(){
        brightnessInfo_ = device::ColorSensor::getInstance();
        touch_ = device::TouchSensor::getInstance();
        display_ = device::Display::getInstance();
        gyroSensor_ = device::GyroSensor::getInstance();
        straightRunning_ = StraightRunning();
    }

    StartUp* StartUp::getInstance(){
        if (!instance_) {
            instance_ = new StartUp();
        }
        return instance_;
    }

    bool StartUp::isFinished(){
        //return selectCourse() && calibrate();
        return selectCourse() && calibrateAutomatically();
    }

    char StartUp::getSelectedCourse(){
        return selectedCourse_;
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

        // タッチセンサの押下状態の更新(長押し中に処理が進まないようにするため)
        if ( hasPressed && touch_->isPressed() ){
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
                brightnessInfo_->setCalibrateValue(whiteValue, blackValue);
            }

        }
        // キャリブレーションの確認
        else if ( !isConfirmed ){
            calibrated = false;
            display_-> updateDisplay("      - CALIBRATION -      ", 0);
            display_-> updateDisplay("CALIBRATION FINISHED        ", 1);

            sprintf( message, "Color (W,B),C: (%d, %d), %d ",
                    brightnessInfo_->getWhiteCalibratedValue(),
                    brightnessInfo_->getBlackCalibratedValue(),
                    brightnessInfo_->getBrightness() );
            display_-> updateDisplay(message, 2);
            if ( touch_->isPressed() ){
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

        // タッチセンサの押下状態の更新(長押し中に処理が進まないようにするため)
        if ( hasPressed &&
                (touch_->isPressed() || ev3_button_is_pressed (UP_BUTTON)
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
                    selectedCourse_ = 'L';
                    break;
                case 1:
                    selectedCourse_ = 'R';
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
            display_-> updateDisplay("                            ", 0);
            display_-> updateDisplay("        SELECT COURSE       ", 1);
            display_-> updateDisplay(courseNames[0], 2);
            display_-> updateDisplay(courseNames[1], 3);
        }
        //コースが選択された時
        else if ( courseSelected && !confirmed ){
            if (ev3_button_is_pressed (ENTER_BUTTON) || touch_->isPressed() ){
                hasPressed = true;
                confirmed = true;
            }

            char message[30];
            sprintf(message, "  SELECTED COURSE: %c", selectedCourse_);
            display_-> updateDisplay("                            ", 0);
            display_-> updateDisplay("                            ", 1);
            display_-> updateDisplay("                            ", 2);
            display_-> updateDisplay(message, 3);
        }

        return courseSelected && confirmed;
    }
    //スタートを受け入れる
    bool StartUp::acceptStart(){
        static bool started = false;

        // タッチセンサの押下状態を保持する静的変数,押下されたらtrue,離れたらfalseにする
        static bool hasPressed = true;
        if  ( !touch_->isPressed() ){
            hasPressed = false;
        }
        if ( !started && !hasPressed && touch_->isPressed()){
            started = true;

            display_-> updateDisplay ("                            ", 0);
            display_-> updateDisplay ("                            ", 1);
            display_-> updateDisplay ("                            ", 2);
            display_-> updateDisplay ("         S T A R T          ", 3);
        }
        else if ( !started ){
            display_-> updateDisplay ("                            ", 0);
            display_-> updateDisplay ("                            ", 1);
            display_-> updateDisplay ("                            ", 2);
            display_-> updateDisplay ("         R E A D Y          ", 3);
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

        display_-> updateDisplay("      - CALIBRATION -      ",  0);
        sprintf( calibrationColorMessage, "Color sensor [%s]       ",colorStr);
        display_->updateDisplay(calibrationColorMessage,1);
        tmpValue = brightnessInfo_->getBrightness();
        sprintf( currentValueMessage, "current value: %2d          ", tmpValue);
        display_-> updateDisplay(currentValueMessage, 2);

        if ( touch_->isPressed() ){
            // 音を出す
            ev3_speaker_play_tone ( 500, 100);
            return tmpValue;
        }

        return -1;
    }

    bool StartUp::calibrateAutomatically(){
        static DistanceMeasurement distanceMeasurement = DistanceMeasurement();
        switch(autoCalibrationState_){
            case AutoCalibrationState::INIT:
                distanceMeasurement.setTargetDistance(100);
                distanceMeasurement.startMeasurement();
                autoCalibrationState_ = AutoCalibrationState::WAIT;
                break;

            case AutoCalibrationState::WAIT:
                display_-> updateDisplay("    - AUTO CALIBRATION -   ", 2);
                display_-> updateDisplay("      PUSH TO START         ", 4);
                if (isClicked()){
                    autoCalibrationState_ = AutoCalibrationState::FORWARD;
                }
                break;

            case AutoCalibrationState::FORWARD:
                display_-> updateDisplay("            FORWARD         ", 4);
                findCalibratedValue();  // キャリブレーション値を探す

                if (runAndStop(30, 40)){
                    autoCalibrationState_ = AutoCalibrationState::STOP;
                    timeMeasurement_ = TimeMeasurement();
                    timeMeasurement_.setBaseTime();
                    timeMeasurement_.setTargetTime(500);

                    // キャリブレーション値をセットする
                    brightnessInfo_->setCalibrateValue(whiteValue_, blackValue_);
                }
                break;

            case AutoCalibrationState::STOP:
                display_-> updateDisplay("            STOP            ", 4);
                if (timeMeasurement_.getResult()){
                    gyroSensor_->reset(); // ジャイロセンサをリセット

                    autoCalibrationState_ = AutoCalibrationState::BACK;
                    distanceMeasurement.setTargetDistance(100);
                    distanceMeasurement.startMeasurement();
                }
                break;

            case AutoCalibrationState::BACK:
                display_-> updateDisplay("            BACK            ", 4);
                if (runAndStop(-30, -40)){
                display_-> updateDisplay("                           ", 2);
                display_-> updateDisplay("                            ", 4);
                autoCalibrationState_ = AutoCalibrationState::FINISHED;
                }
                break;

            case AutoCalibrationState:: FINISHED:
                {
                    display_-> updateDisplay("CALIBRATION FINISHED        ", 2);

                    char message[30];
                    sprintf( message, "Color (W,B),C: (%d, %d), %d ",
                            brightnessInfo_->getWhiteCalibratedValue(),
                            brightnessInfo_->getBlackCalibratedValue(),
                            brightnessInfo_->getBrightness() );
                    display_-> updateDisplay(message, 4);
                }
                return true;
        }
        return false;
    }

    bool StartUp::isClicked(){
        static bool hasPressed = false;
        if (touch_->isPressed()){
            hasPressed = true;
        }
        else if (hasPressed){
            hasPressed = false;
            return true;
        }
        return false;
    }

    bool StartUp::changeSpeed(int currentPwm, int targetPwm, int acceleration){
        static Clock clock = Clock();
        static bool initialized = false;
        if ( !initialized ){
            initialized = true;
            currentTimeMs_ = clock.now();
            currentPwm_ = currentPwm;
            return false;
        }

        // 加速度[pwm/s] * 時間[s]
        double diff = (double)acceleration * (double)(clock.now() - currentTimeMs_) / 1000;
        currentTimeMs_ = clock.now();
        currentPwm_ += diff;

        bool speedChanged = false;
        if ( (0 < acceleration && targetPwm < currentPwm_) ||
                (0 > acceleration && targetPwm > currentPwm_) ){
            currentPwm_ = targetPwm;
            speedChanged = true;
            initialized = false; // 初期化
        }

        straightRunning_.run((int)currentPwm_);
        return speedChanged;
    }

    bool StartUp::runAndStop(int maxPwm, int acceleration){
        static bool isAccelerating = true;
        bool isFinished = false;
        if (isAccelerating && changeSpeed(0, maxPwm, acceleration)){
            isAccelerating = false;
        }
        else if ( !isAccelerating && changeSpeed(maxPwm, 0, -acceleration)){
            isAccelerating = true; // 初期化
            isFinished = true;
        }
        return isFinished;
    }

    void StartUp::findCalibratedValue(){
        static bool initialized = false;
        int brightness = brightnessInfo_->getBrightness();
        if (!initialized){
            initialized = true;
            whiteValue_ = brightness;
            blackValue_ = brightness;
        }

        if (brightness > whiteValue_){
            whiteValue_ = brightness;
        }
        if (brightness < blackValue_){
            blackValue_ = brightness;
        }
    }
}
