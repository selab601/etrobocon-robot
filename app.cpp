#include <stdio.h>
#include "ev3api.h"
#include "app.h"
#include "device/Display.h"
#include "device/Motors.h"
#include "device/TouchSensor.h"
#include "measurement/SelfPositionEstimation.h"
#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

using namespace device;
using namespace measurement;

extern void main_task(intptr_t unused);

void main_task(intptr_t unused)
{
    /* LCD画面表示 */
    Display* disp = Display::getInstance();
    SelfPositionEstimation* loc = SelfPositionEstimation::getInstance();
    Motors* mtrs = Motors::getInstance();
    device::TouchSensor* tch = device::TouchSensor::getInstance();
    /**
     * Main loop
     */
    while(1) {
        tslp_tsk(4); /* 4msec 周期起動 */
        //自己位置の更新
        loc->update(mtrs->getCount(MOTOR_LEFT),mtrs->getCount(MOTOR_RIGHT));
        //距離をディスプレイに表示
        disp->updateDisplay("MigrationLength",loc->getMigrationLength(),1);
        disp->updateDisplay("Angle",loc->getAngle(),2);
        if(tch->isPressed()){
            mtrs->setWheelPWM(-10,10);
        }
        if(loc->getAngle() > 360){
            mtrs->setWheelPWM(0,0);
        }
    }

    ext_tsk();
}
