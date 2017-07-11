#ifndef ET2016_COMMUNICATION_BTMANAGER_H_
#define ET2016_COMMUNICATION_BTMANAGER_H_

#include <stdio.h>
#include <string.h>
#include <Clock.h>
#include "ev3api.h"
#include "../device/ColorSensor.h"
#include "../device/GyroSensor.h"
#include "../device/Motors.h"
#include "../device/SonarSensor.h"
#include "../device/TouchSensor.h"
#include "../measurement/SelfPositionEstimation.h"
#include "../device/Button.h"

/* メッセージの長さ */
#define MESSAGE_LEN  500

namespace communication
{

    /**
     * @brief Bluetooth通信用クラスです。
     * connect関数で接続されるまで待機します。
     * send関数でデータを送信します。
     */
    class BtManager
    {
        public:
            /* 接続状態の列挙型 */
            enum BT_STATE
            {
                BT_CONNECTED,
                BT_CLOSED,
                BT_WAITING,
            };

        private:
            /* このクラスのインスタンス */
            static BtManager* instance_;

            /* 通信先 */
            FILE* btSerialPort_;

            /* 時間情報 */
            ev3api::Clock* clock_;

            /* 光情報 */
            device::ColorSensor* color_;

            /* ジャイルセンサ情報 */
            device::GyroSensor* gyro_;

            /* ソナーセンサ情報 */
            device::SonarSensor* sonar_;

            /* タッチセンサ情報 */
            device::TouchSensor* touch_;

            /* モータ情報 */
            device::Motors* motors_;

            /* 自己位置情報 */
            measurement::SelfPositionEstimation* selfPositionEstimation_;

            /* 送信メッセージ */
            char message_[MESSAGE_LEN];

            /* 接続状態 */
            BT_STATE state_;

        public:
            /**
             * @brief インスタンス取得
             * @return Bluetooth管理用インスタンス
             */
            static BtManager* getInstance();

            /**
             * @brief Bluetooth切断
             */
            void close();

            /**
             * @brief Bluetooth接続
             * 接続されるまで待機します
             * @return 成功したらtrue
             */
            bool connect();

            /**
             * @brief 接続状態を返す
             * BT_WATING : 接続待機
             * BT_CONNECTED : 接続中 データ送信出来ます
             * BT_CLOSED : 通信終了済み 以降データ送信を行いません
             */
            BT_STATE getState();

            /**
             * @brief Bluetooth接続確認
             * @return 接続中ならtrue
             */
            bool isConnected();

            /**
             * @brief メッセージ送信
             * 走行体情報を送ります
             * setMessageによってメッセージがセットされている場合はそのメッセージを、
             * されていない場合はデフォルトのメッセージを送信します
             */
            void send();

            /**
             * @brief メッセージ受信
             * メッセージを受信します
             */
            void receive();

            /**
             * @brief 送信データのセット
             * 送信メッセージをセットします
             */
            void setMessage(const char[]);

        private:
            /**
             * @brief コンストラクタ
             * 外部から呼び出せません。
             */
            BtManager();

    };
}
#endif
