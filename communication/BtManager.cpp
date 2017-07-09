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
            sprintf(message_,
                    "{\"clock\":%ld,\"gyro\":%d,\"touch\":%d,\"sonar\":%d,"
                    "\"brightness\":%d,"
                    "\"rgb_r\":%d,\"rgb_g\":%d,\"rgb_b\":%d,"
                    "\"hsv_h\":%d,\"hsv_s\":%d,\"hsv_v\":%d,"
                    "\"arm_count\":%ld,\"left_count\":%ld,\"right_count\":%ld,"
                    "\"length\":%ld,\"angle\":%d,"
                    "\"coordinate_x\":%ld,\"coordinate_y\":%ld}\n",
                    clock_->now(),
                    gyro_->getAnglerVelocity(),
                    touch_->isPressed() ? 1 : 0,
                    sonar_ != 0? sonar_->getDistance(): 0,
                    color_->getBrightness(),
                    color_->getR(), // color r
                    color_->getG(), // color g
                    color_->getB(), // color b
                    color_->getH(), // color h
                    color_->getS(), // color s
                    color_->getV(), // color v
                    motors_->getCount(device::MOTOR_ARM),
                    motors_->getCount(device::MOTOR_LEFT),
                    motors_->getCount(device::MOTOR_RIGHT),
                    selfPositionEstimation_->getMigrationLength(),
                    selfPositionEstimation_->getAngle(),
                    selfPositionEstimation_->getLocationX(),
                    selfPositionEstimation_->getLocationY()
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
