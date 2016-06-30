#include "BtManager.h"

namespace communication {

	/* メンバ */
	BtManager* BtManager::instance = 0;
	// FILE* BtManager::bt;
	// char BtManager::message[] = {0};

	/* コンストラクタ */
	BtManager::BtManager() {
  clock = new Clock();
		color = Color::getInstance();
		gyro  = Gyro::getInstance();
		touch = Touch::getInstance();
		motor = TrikeMotor::getInstance();
		loc = Localization::getInstance();
		bt = NULL;
		state = BT_WATING;
	}

	/* インスタンス取得 */
	BtManager* BtManager::getInstance() {
		if (instance == 0) {
			instance = new BtManager();
		}
		return instance;
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
	    bt = ev3_serial_open_file(EV3_SERIAL_BT);

	    /* 接続開始送信 */
     //char msg[10] = {0};
      //sprintf(msg, "start\r\n");
      //setMessage(msg);

      state = BT_CONNECTED;
	  }
	  return true;
	}

	/* Bluetooth接続確認 */
	bool BtManager::isConnected() {
		return ev3_bluetooth_is_connected();
	}

 /* Bluetooth切断 */
	void BtManager::close() {
		state = BT_CLOSED;
		if (bt != NULL) {
			fclose(bt);
		}
	}


	/* 走行体情報送信 */
	void BtManager::send() {
		/* メッセージがなかったら情報取得 */
		if (message[0] == '\0') {
      /*
			sprintf(message, "bligh:%.3d, touch:%.1d, gyro :% .3d, sonar:%.3d, dist :%.8ld\r\n",
			        color->getBrightness(),
			        touch->isPressed() ? 1 : 0,
			        gyro->getGyroValue(),
			        sonar->getSonarValue(),
			        loc->get_migrationLength());
      */
      sprintf(message, "%ld,%d,%d,%d,%d,%ld,%ld,%ld,%d,%d,%d,%ld,%ld,%d\n",
              clock->now(),
              color->getBrightness(),
              gyro->getGyroValue(),
              0,
              touch->isPressed() ? 1 : 0,
              motor->getFrontCount(),
              motor->getLeftCount(),
              motor->getRightCount(),
              0,
              0,
              0,
              motor->getFrontAngle(),
              loc->get_migrationLength(),
              loc->getAngle());
		}

		/* 送信 */
		int size = sizeof(message);
		int result;
		if (size > 0) {
		  result = fprintf(bt, message);
		}

		/* 送信失敗したら通信終了 */
		if (result < 0) {
			close();
		}

		/* メッセージ初期化 */
		for (int i = 0; i < MESSAGE_LEN; i++) {
			message[i] = '\0';
		}
	}

    /* メッセージ受信 */
    void BtManager::receive() {
        //        if (touch->isPressed()) {
        char r_tmp[MESSAGE_LEN];
        //int size = fscanf(bt, "%s", r_tmp);
        int size = fread(r_tmp, 1, MESSAGE_LEN - 1, bt);
        //fseek(bt, 0, SEEK_END);
        //int size = ftell(bt);
        //fgets(r_tmp, MESSAGE_LEN - 1, bt);
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
		sprintf(message, "%s", msg);
	}

	/* ステータス更新 */
	BtManager::BT_STATE BtManager::getState() {
		return state;
	}
}
