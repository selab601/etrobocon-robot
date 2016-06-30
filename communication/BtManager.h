#ifndef ET2015_COMMUNICATION_BTMANAGER_H_
#define ET2015_COMMUNICATION_BTMANAGER_H_

#include <stdio.h>
#include <string.h>
#include <Clock.h>
#include "ev3api.h"
#include "../device/Color.h"
#include "../device/Gyro.h"
#include "../device/Sonar.h"
#include "../device/Touch.h"
#include "../device/TrikeMotor.h"
#include "../underlyingTechnology/Localization.h"

/* メッセージの長さ */
#define MESSAGE_LEN  128

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
                BT_WATING,
            };

        private:
            /* このクラスのインスタンス */
            static BtManager* instance;

            /* 通信先 */
            FILE* bt;

            /* 時間情報 */
            Clock* clock;

            /* 光情報 */
            Color* color;

            /* ジャイルセンサ情報 */
            Gyro* gyro;

            /* ソナーセンサ情報 */
            Sonar* sonar;

            /* タッチセンサ情報 */
            Touch* touch;

            /* モータ情報 */
            TrikeMotor* motor;

            /* 走行情報 */
            Localization* loc;

            /* 送信メッセージ */
            char message[MESSAGE_LEN];

            /* 受信メッセージ */
            char r_message[MESSAGE_LEN];

            /* 接続状態 */
            BT_STATE state;

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
             * @brief メッセージ送信
             * 改行またはnullまでを送信
             */
            // void send(const char[]);

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
