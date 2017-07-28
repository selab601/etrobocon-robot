#include "StartUp.h"

#define COURSES_NUM 	2 // コースの数

using namespace device;
using namespace drive;
using namespace measurement;

namespace contest_pkg{

    StartUp* StartUp::instance_ = 0;

    StartUp::StartUp(){
        brightnessInfo_ = device::ColorSensor::getInstance();
        touch_ = device::TouchSensor::getInstance();
        display_ = device::Display::getInstance();
        gyroSensor_ = device::GyroSensor::getInstance();
    }

    StartUp* StartUp::getInstance(){
        if (!instance_) {
            instance_ = new StartUp();
        }
        return instance_;
    }

    bool StartUp::start(){
        switch (state_){
            case State::INIT:
                display_->clearDisplay();
                state_ = State::SELECT_COURCE;
                break;
            case State::SELECT_COURCE:
                if (selectCourse()){
                    if ('L' == getSelectedCourse()){
                        state_ = State::INPUT_BLOCK_CODE;
                    }
                    else{
                        state_ = State::CALIBRATE;
                    }
                }
                break;

            case State::INPUT_BLOCK_CODE:
                if (BlockInputUI::getInstance()->input()){
                    state_ = State::CALIBRATE;
                }
                break;

            case State::CALIBRATE:
                if (calibrateAutomatically()){
                    display_->clearDisplay();
                    state_ = State::ACCEPT_START;
                }
                break;

            case State::ACCEPT_START:
                if (acceptStart()){
                    state_ = State::FINISHED;
                }
                break;

            case State::FINISHED:
                return true;
        }
        return false;
    }
    bool StartUp::isFinished(){
        return start();
    }

    char StartUp::getSelectedCourse(){
        return selectedCourse_;
    }
    bool StartUp::selectCourse(){
        static int index = 0;
        static bool courseSelected = false;
        static Buttons* btn = Buttons::getInstance();
        if (courseSelected) // 選択が終わったら何もしない
            return true;

        // ↑ ボタンが押されたとき
        if (btn->upClicked()){
            index--;
        }
        // ↓ ボタンが押されたとき
        else if (btn->downClicked()){
            index++;
        }
        // エンターボタンが押されたとき
        else if (btn->enterClicked()){
            courseSelected = true;
            switch( index ){
                case 0:
                    selectedCourse_ = 'L';
                    break;
                case 1:
                    selectedCourse_ = 'R';
                    break;
            }
            // ディスプレイを消す
            display_->clearDisplay();
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

        // 選択されたコースでLEDの色を変える
        switch( index ){
            case 0:
                ev3_led_set_color (LED_RED); // Lのとき赤
                break;
            case 1:
                ev3_led_set_color (LED_ORANGE); // Rのときオレンジ
                break;
        }

        char courseNames[COURSES_NUM][15] = {"   Course L","   Course R"};
        courseNames[index][1] = '>';

        //コースが選択されていないとき
        if ( !courseSelected ){
            display_-> updateDisplay("                            ", 0);
            display_-> updateDisplay("        SELECT COURSE       ", 1);
            display_-> updateDisplay(courseNames[0], 2);
            display_-> updateDisplay(courseNames[1], 3);
        }
        return false;
    }

    //スタートを受け入れる
    bool StartUp::acceptStart(){
        static bool accepted = false;
        if (!accepted && touch_->isClicked()){
            display_->clearDisplay();
            display_-> updateDisplay ("         S T A R T          ", 3);
            accepted = true;
        }
        else if (!accepted){
            display_-> updateDisplay ("         R E A D Y          ", 3);
            sprintf(message_, "   Selected Cource :%c       ", getSelectedCourse());
            display_->updateDisplay(message_, 5);
            display_->updateDisplay("Color W,B,C",
                    brightnessInfo_->getWhiteCalibratedValue(),
                    brightnessInfo_->getBlackCalibratedValue(),
                    brightnessInfo_->getBrightness(), 6);
            if ('L' == getSelectedCourse()){
                display_->updateDisplay("Block Code",
                        BlockInputUI::getInstance()->getCode(), 7);
            }
        }
        return accepted;
    }

    bool StartUp::calibrateAutomatically(){
        static DistanceMeasurement distanceMeasurement = DistanceMeasurement();
        static TimeMeasurement timeMeasurement = TimeMeasurement();

        switch(autoCalibrationState_){
            case AutoCalibrationState::INIT:
                distanceMeasurement.setTarget(100);
                distanceMeasurement.start();
                autoCalibrationState_ = AutoCalibrationState::WAIT;
                break;

            case AutoCalibrationState::WAIT:
                display_-> updateDisplay("    - AUTO CALIBRATION -   ", 2);
                display_-> updateDisplay("      PUSH TO START         ", 4);
                if (touch_->isClicked()){
                    autoCalibrationState_ = AutoCalibrationState::ADJUST_ARM;
                }
                break;

            case AutoCalibrationState::ADJUST_ARM:
                if (Arm::getInstance()->reset()){
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
                    timeMeasurement.setTargetTime(50);

                    // キャリブレーション値をセットする
                    brightnessInfo_->setCalibrateValue(whiteValue_, blackValue_);
                }
                break;

            case AutoCalibrationState::STOP:
                display_-> updateDisplay("            STOP            ", 4);
                if (timeMeasurement.getResult()){
                    autoCalibrationState_ = AutoCalibrationState::BACK;
                    distanceMeasurement.setTarget(100);
                    distanceMeasurement.start();
                }
                break;

            case AutoCalibrationState::BACK:
                Shippo::getInstance()->furifuri();
                display_-> updateDisplay("            BACK            ", 4);
                if (runAndStop(-30, -40)){
                autoCalibrationState_ = AutoCalibrationState::SHOW_RESULT;
                // 終了したら音を出す
                ev3_speaker_play_tone ( 500, 100);
                }
                break;

            case AutoCalibrationState::SHOW_RESULT:
                {
                    Shippo::getInstance()->furifuri();
                    display_-> updateDisplay("  CALIBRATION FINISHED  ", 2);

                    display_->updateDisplay("Color W,B,C",
                            brightnessInfo_->getWhiteCalibratedValue(),
                            brightnessInfo_->getBlackCalibratedValue(),
                            brightnessInfo_->getBrightness(), 4);

                    if (touch_->isClicked()){
                        autoCalibrationState_ = AutoCalibrationState::STOP_FURIFURI;
                    display_-> updateDisplay("                           ", 2);
                    display_-> updateDisplay("                           ", 4);
                    }
                }
                break;

            case AutoCalibrationState::STOP_FURIFURI:
                if (Shippo::getInstance()->pleased()){
                    autoCalibrationState_ = AutoCalibrationState::FINISHED;
                }
                break;

            case AutoCalibrationState:: FINISHED:
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

        StraightRunning straightRunning = StraightRunning();
        straightRunning.run((int)currentPwm_);
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
