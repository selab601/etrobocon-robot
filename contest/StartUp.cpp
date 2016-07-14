#include "StartUp.h"
#include "stdio.h"
#include "../measurement/DistanceMeasurement.h"
#include "../device/Motors.h"
#include <Clock.h>

#define COURSES_NUM 	2 // コースの数

using namespace drive;
using namespace measurement;

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
        return selectCourse() && calibrateAutomatically();
    }

    char StartUp::getSelectedCourse(){
        return selectedCourse_;
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
                display_-> updateDisplay("                            ", 1);
                display_-> updateDisplay("                            ", 2);
                display_-> updateDisplay("                            ", 3);
                hasPressed = true;
                confirmed = true;
            }
        }

        return courseSelected && confirmed;
    }

    //スタートを受け入れる
    bool StartUp::acceptStart(){
        static bool started = false;
        if ( !started && isClicked()){
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

    bool StartUp::calibrateAutomatically(){
        static DistanceMeasurement distanceMeasurement = DistanceMeasurement();
        static TimeMeasurement timeMeasurement = TimeMeasurement();

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
                    autoCalibrationState_ = AutoCalibrationState::ADJUST_ARM;
                }
                break;

            case AutoCalibrationState::ADJUST_ARM:
                if (setArmAngle()){
                    autoCalibrationState_ = AutoCalibrationState::FORWARD;
                }
                break;

            case AutoCalibrationState::FORWARD:
                display_-> updateDisplay("            FORWARD         ", 4);
                findCalibratedValue();  // キャリブレーション値を探す

                if (runAndStop(30, 40)){
                    autoCalibrationState_ = AutoCalibrationState::STOP;
                    timeMeasurement = TimeMeasurement();
                    timeMeasurement.setBaseTime();
                    timeMeasurement.setTargetTime(500);

                    // キャリブレーション値をセットする
                    brightnessInfo_->setCalibrateValue(whiteValue_, blackValue_);
                }
                break;

            case AutoCalibrationState::STOP:
                display_-> updateDisplay("            STOP            ", 4);
                if (timeMeasurement.getResult()){
                    gyroSensor_->reset(); // ジャイロセンサをリセット

                    autoCalibrationState_ = AutoCalibrationState::BACK;
                    distanceMeasurement.setTargetDistance(100);
                    distanceMeasurement.startMeasurement();
                }
                break;

            case AutoCalibrationState::BACK:
                display_-> updateDisplay("            BACK            ", 4);
                if (runAndStop(-30, -40)){
                autoCalibrationState_ = AutoCalibrationState::SHOW_RESULT;
                // 終了したら音を出す
                ev3_speaker_play_tone ( 500, 100);
                }
                break;

            case AutoCalibrationState::SHOW_RESULT:
                {
                    display_-> updateDisplay("  CALIBRATION FINISHED  ", 2);

                    char message[30];
                    sprintf( message, "Color (W,B),C: (%d, %d), %d ",
                            brightnessInfo_->getWhiteCalibratedValue(),
                            brightnessInfo_->getBlackCalibratedValue(),
                            brightnessInfo_->getBrightness() );
                    display_-> updateDisplay(message, 4);
                    if (isClicked()){
                        autoCalibrationState_ = AutoCalibrationState::FINISHED;
                    display_-> updateDisplay("                           ", 2);
                    display_-> updateDisplay("                           ", 4);
                    }
                }
                break;

            case AutoCalibrationState:: FINISHED:
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
            // クリックされたら音を出す
            ev3_speaker_play_tone ( 500, 100);
            return true;
        }
        return false;
    }

    bool StartUp::changeSpeed(int currentPwm, int targetPwm, int acceleration){
        static ev3api::Clock clock = ev3api::Clock();
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

    bool StartUp::setArmAngle(){
        static TimeMeasurement timeMeasurement = TimeMeasurement();
        static int baseCount = 0;
        device::Motors* motors = device::Motors::getInstance();
        switch(armSettingState_){
            case ArmSettingState::INIT:
                display_-> updateDisplay("    - ARM ADJUSTING -      ", 2);
                timeMeasurement.setTargetTime(800);
                timeMeasurement.setBaseTime();
                armSettingState_ = ArmSettingState::PULL;
                break;

            case ArmSettingState::PULL:
                motors->setPWM(device::MOTOR_ARM, -40);
                if (timeMeasurement.getResult()){
                    armSettingState_ = ArmSettingState::PUSH;
                    motors->setPWM(device::MOTOR_ARM, 0);
                    baseCount = motors->getCount(device::MOTOR_ARM);
                }
                break;

            case ArmSettingState::PUSH:
                {
                    int currentCount = motors->getCount(device::MOTOR_ARM);
                    int pwm = ARM_ANGLE - (currentCount - baseCount);
                    motors->setPWM(device::MOTOR_ARM, pwm);

                    if (ARM_ANGLE == (currentCount - baseCount)){
                        armSettingState_ = ArmSettingState::FINISHED;
                        motors->reset(); // モータのエンコーダ値をリセット
                        ev3_speaker_play_tone(500,100); // 終わった時に音を出す
                        display_-> updateDisplay("                           ", 2);
                    }
                }
                break;

            case ArmSettingState::FINISHED:
                return true;
                break;
        }
        return false;
    }
}
