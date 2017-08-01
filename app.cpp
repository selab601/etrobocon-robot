#include <stdio.h>
#include "ev3api.h"
#include "app.h"
#include "./device/Display.h"
#include "./contest/Contest.h"
#include "./communication/BtManager.h"
#include "./measurement/SelfPositionEstimation.h"
#include "./device/Motors.h"

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
    disp->updateDisplay("ET2017!!", 1);

    /**
     * Main loop
     */
    while(1) {
        contest_pkg::Contest::getInstance()->perform();
        tslp_tsk(4); /* 4msec 周期起動 */
    }

    ext_tsk();
}

/* 自己位置更新タスク */
void self_position_update_task(intptr_t unused)
{
    while(1){
        tslp_tsk(4); /* 100msec 周期起動 */
        device::Motors* motors = device::Motors::getInstance();
        int32_t lCount = motors->getCount(device::MOTOR_LEFT);
        int32_t rCount = motors->getCount(device::MOTOR_RIGHT);
        measurement::SelfPositionEstimation::getInstance()->update(lCount, rCount);
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

void* __dso_handle=0;
