#include <stdio.h>
#include "ev3api.h"
#include "app.h"
#include "./device/Display.h"
#include "./communication/BtManager.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

using namespace device;

extern void main_task(intptr_t unused);

void main_task(intptr_t unused)
{
    /* LCD画面表示 */
    Display* disp = Display::getInstance();
    disp->updateDisplay("ET2016!!", 1);

    /**
     * Main loop
     */
    while(1) {
        tslp_tsk(4); /* 4msec 周期起動 */
    }

    ext_tsk();
}

/* Bluetoothタスク */
void bt_task(intptr_t unused)
{
    communication::BtManager* btManager = communication::BtManager::getInstance();

    while(1){
        tslp_tsk(100); /* 100msec 周期起動 */

        if (btManager->getState() == btManager->BT_WAITING) {
            /* 接続 */
            btManager->connect();
        } else if(btManager->getState() == btManager->BT_CONNECTED) {
            /* メッセージ送信 */
            btManager->send();
        }
    }

    ext_tsk();
}
