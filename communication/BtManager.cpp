#include "BtManager.h"

using namespace ev3api;

namespace communication {

    /* メンバ */
    BtManager* BtManager::instance_ = 0;

    /* コンストラクタ */
    BtManager::BtManager() {
        clock_  = new Clock();
        color_  = device::ColorSensor::getInstance();
        gyro_   = device::GyroSensor::getInstance();
        touch_  = device::TouchSensor::getInstance();
        sonar_ = device::SonarSensor::getInstance();
        motors_ = device::Motors::getInstance();
        selfPositionEstimation_ = measurement::SelfPositionEstimation::getInstance();
        btSerialPort_ = NULL;
        state_ = BT_WAITING;
    }

    /* インスタンス取得 */
    BtManager* BtManager::getInstance() {
        if (instance_ == 0) {
            instance_ = new BtManager();
        }
        return instance_;
    }

    /* Bluetooth接続 */
    bool BtManager::connect() {
        /* つないでなかったら接続 */
        if (!ev3_bluetooth_is_connected()) {
            /* 音 */
            ev3_speaker_set_volume(1);
            ev3_speaker_play_tone(500, 100);

            /* 画面 */
            ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
            ev3_lcd_draw_string("connecting ... ", 0, 0);
            ev3_lcd_draw_string("Push enter to exit", 0, 10);

            /* 接続待機 */
            bool pressState = false;
            while(!ev3_bluetooth_is_connected()) {
                if (ev3_button_is_pressed(ENTER_BUTTON)) {
                    pressState = true;
                } else {
                    // ボタンが離されていて，且つ過去に押されていたら
                    if (pressState == true) {
                        close();
                        return false;
                    }
                }
            }

            /* 音 */
            ev3_speaker_play_tone(500, 100);

            /* 画面 */
            ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
            ev3_lcd_draw_string("connected !", 0, 0);

            /* 接続先ポート(を、ファイルとして開く) */
            btSerialPort_ = ev3_serial_open_file(EV3_SERIAL_BT);

            state_ = BT_CONNECTED;
        }
        return true;
    }

    /* Bluetooth接続確認 */
    bool BtManager::isConnected() {
        return ev3_bluetooth_is_connected();
    }

    /* Bluetooth切断 */
    void BtManager::close() {
        state_ = BT_CLOSED;
        if (btSerialPort_ != NULL) {
            fclose(btSerialPort_);
        }
    }


    /* 走行体情報送信 */
    void BtManager::send() {
        /* メッセージがなかったら情報取得 */
        if (message_[0] == '\0') {
            sprintf(message_, "%ld,%d,%d,%d,%d,%ld,%ld,%ld,%d,%d,%d,%ld,%ld,%d\n",
                    clock_->now(),
                    color_->getBrightness(),
                    gyro_->getAnglerVelocity(),
                    0,
                    touch_->isPressed() ? 1 : 0,
                    motors_->getCount(device::MOTOR_ARM),
                    motors_->getCount(device::MOTOR_LEFT),
                    motors_->getCount(device::MOTOR_RIGHT),
                    sonar_ != 0? sonar_->getDistance(): 0,
                    0,
                    0,
                    (long int)0,
                    selfPositionEstimation_->getMigrationLength(),
                    selfPositionEstimation_->getAngle()
                );
        }

        /* 送信 */
        int size = sizeof(message_);
        int result;
        if (size > 0) {
            result = fprintf(btSerialPort_, message_);
        }

        /* 送信失敗したら通信終了 */
        if (result < 0) {
            close();
        }

        /* メッセージ初期化 */
        for (int i = 0; i < MESSAGE_LEN; i++) {
            message_[i] = '\0';
        }
    }

    /* メッセージ受信 */
    void BtManager::receive() {
        char r_tmp[MESSAGE_LEN];
        int size = fread(r_tmp, 1, MESSAGE_LEN - 1, btSerialPort_);
        ev3_speaker_play_tone(500, 100);
        if (size > 0) {
            ev3_speaker_play_tone(500, 100);
        }
    }

    /* 指定メッセージ送信 */
    void BtManager::setMessage(const char msg[]) {
        sprintf(message_, "%s", msg);
    }

    /* ステータス更新 */
    BtManager::BT_STATE BtManager::getState() {
        return state_;
    }
}