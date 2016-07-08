#include "display.h"

namespace device
{
    // インスタンス変数初期化
    Display* Display::instance_ = 0;

    // シングルトン
    Display* Display::getInstance(){
        if(!instance_){
            instance_ = new Display();
        }
        return instance_;
    }

    Display::Display(){
        // フォントの指定
        setFont(EV3_FONT_SMALL);
    }

    void Display::setFont(lcdfont_t fontType){
        // フォントのセット
        ev3_lcd_set_font(fontType);
        // フォントサイズの取得
        ev3_font_get_size(fontType,
                          &fontWidth_,
                          &fontHeight_);
        // 表示可能な文字数計算
        maxOutputChars_ = EV3_LCD_WIDTH/fontWidth_;
        outputStrBuffer_ =
            (char *)malloc(sizeof(char) * maxOutputChars_);
    }

    void Display::clearDisplay(){
        // 白で矩形塗りつぶし
        ev3_lcd_fill_rect(0,
                          0,
                          EV3_LCD_WIDTH,
                          EV3_LCD_HEIGHT,
                          EV3_LCD_WHITE);
        free(outputStrBuffer_);
    }

    void Display::updateDisplay(const char* str, int col){
        if (strlen(str) > maxOutputChars_){
            ev3_lcd_draw_string("message to output is too long", 0, fontHeight_ * col);
        }

        sprintf(outputStrBuffer_, "%s", str);
        ev3_lcd_draw_string(outputStrBuffer_, 0, fontHeight_ * col);
    }

    void Display::updateDisplay(const char* str, int num, int col){
        if (strlen(str)+6 > maxOutputChars_){
            ev3_lcd_draw_string("message to output is too long", 0, fontHeight_ * col);
        }

        sprintf(outputStrBuffer_, "%s : %6d", str, num);
        ev3_lcd_draw_string(outputStrBuffer_, 0, fontHeight_ * col);
    }
}
