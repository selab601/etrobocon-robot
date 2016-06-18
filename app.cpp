/**
 * @file app.cpp
 * @brief メインファイル
 */

#include <stdio.h>
#include "ev3api.h"
#include "app.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

extern void main_task(intptr_t unused);

void main_task(intptr_t unused)
{
    /* LCD画面表示 */
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    ev3_lcd_draw_string("ET2016!!", 0, 8);

    /**
     * Main loop
     */
    while(1) {
        tslp_tsk(4); /* 4msec 周期起動 */
    }

    ext_tsk();
}
