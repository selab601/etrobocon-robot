#include "BtManager.h"

using namespace ev3api;

namespace communication {

    /* メンバ */
    BtManager* BtManager::mInstance = 0;
    // FILE* BtManager::mBtSerialPort;
    // char BtManager::mMessage[] = {0};

    /* コンストラクタ */
    BtManager::BtManager() {
        mClock  = new Clock();
        mColor  = device::ColorSensor::getInstance();
        mGyro   = device::GyroSensor::getInstance();
        mTouch  = device::TouchSensor::getInstance();
        mSonar = device::SonarSensor::getInstance();
        mMotors = device::Motors::getInstance();
        // loc = Localization::getInstance(); // TODO: enable
        mBtSerialPort = NULL;
        mState = BT_WAITING;
    }

    /* インスタンス取得 */
    BtManager* BtManager::getInstance() {
        if (mInstance == 0) {
            mInstance = new BtManager();
        }
        return mInstance;
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
            int i = 0;
            while(!ev3_bluetooth_is_connected()) {
                if (ev3_button_is_pressed(ENTER_BUTTON)) {
                    i++;
                } else if (i > 0) {
                    close();
                    return false;
                }
            }

            /* 音 */
            ev3_speaker_play_tone(500, 100);

            /* 画面 */
            ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
            ev3_lcd_draw_string("connected !", 0, 0);

            /* 接続先ポート(を、ファイルとして開く) */
            mBtSerialPort = ev3_serial_open_file(EV3_SERIAL_BT);

            /* 接続開始送信 */
            //char msg[10] = {0};
            //sprintf(msg, "start\r\n");
            //setMessage(msg);

            mState = BT_CONNECTED;
        }
        return true;
    }

    /* Bluetooth接続確認 */
    bool BtManager::isConnected() {
        return ev3_bluetooth_is_connected();
    }

    /* Bluetooth切断 */
    void BtManager::close() {
        mState = BT_CLOSED;
        if (mBtSerialPort != NULL) {
            fclose(mBtSerialPort);
        }
    }


    /* 走行体情報送信 */
    void BtManager::send() {
        /* メッセージがなかったら情報取得 */
        if (mMessage[0] == '\0') {
            sprintf(mMessage, "%ld,%d,%d,%d,%d,%ld,%ld,%ld,%d,%d,%d,%ld,%ld,%d\n",
                    mClock->now(),
                    mColor->getBrightness(),
                    mGyro->getAnglerVelocity(),
                    0,
                    mTouch->isPressed() ? 1 : 0,
                    mMotors->getCount(device::MOTOR_ARM),
                    mMotors->getCount(device::MOTOR_LEFT),
                    mMotors->getCount(device::MOTOR_RIGHT),
                    mSonar != 0? mSonar->getDistance(): 0,
                    0,
                    0,
                    (long int)0,
                    (long int)0,
                    0
                    // loc->get_migrationLength(),
                    // loc->getAngle());
                );
        }

        /* 送信 */
        int size = sizeof(mMessage);
        int result;
        if (size > 0) {
            result = fprintf(mBtSerialPort, mMessage);
        }

        /* 送信失敗したら通信終了 */
        if (result < 0) {
            close();
        }

        /* メッセージ初期化 */
        for (int i = 0; i < MESSAGE_LEN; i++) {
            mMessage[i] = '\0';
        }
    }

    /* メッセージ受信 */
    void BtManager::receive() {
        //        if (mTouch->isPressed()) {
        char r_tmp[MESSAGE_LEN];
        //int size = fscanf(mBtSerialPort, "%s", r_tmp);
        int size = fread(r_tmp, 1, MESSAGE_LEN - 1, mBtSerialPort);
        //fseek(mBtSerialPort, 0, SEEK_END);
        //int size = ftell(mBtSerialPort);
        //fgets(r_tmp, MESSAGE_LEN - 1, mBtSerialPort);
        ev3_speaker_play_tone(500, 100);
        if (size > 0) {
            //strcpy(r_message, r_tmp);
            //ev3_speaker_play_tone(300, 100);
            //ev3_speaker_play_tone(400, 100);
            ev3_speaker_play_tone(500, 100);
        }
        //}
    }



    /* 指定メッセージ送信 */
    void BtManager::setMessage(const char msg[]) {
        sprintf(mMessage, "%s", msg);
    }

    /* ステータス更新 */
    BtManager::BT_STATE BtManager::getState() {
        return mState;
    }
}
