/**
 * @file display.cpp
 * @author 兎澤 佑
 */

#include "display.h"

Display* Display::instance_ = 0;

Display* Display::getInstance(){
    if(!instance_){
        instance_ = new Display();
    }
    return instance_;
}

Display::Display(){
    setFont(EV3_FONT_SMALL);
}

void Display::setFont(lcdfont_t font_type){
    // フォントのセット
    ev3_lcd_set_font(font_type);
    // フォントサイズの取得
    ev3_font_get_size(font_type,
                      &font_width_,
                      &font_height_);
    // 表示可能な文字数計算
    max_output_chars_ = EV3_LCD_WIDTH/font_width_;
    output_str_buffer_ =
        (char *)malloc(sizeof(char) * max_output_chars_);
}

void Display::clearDisplay(){
    // 白で矩形塗りつぶし
    ev3_lcd_fill_rect(0,
                      0,
                      EV3_LCD_WIDTH,
                      EV3_LCD_HEIGHT,
                      EV3_LCD_WHITE);
    free(output_str_buffer_);
}

void Display::updateDisplay(const char* str, int col){
    if (strlen(str) > max_output_chars_){
        ev3_lcd_draw_string("message to output is too long", 0, font_height_ * col);
    }

    sprintf(output_str_buffer_, "%s", str);
    ev3_lcd_draw_string(output_str_buffer_, 0, font_height_ * col);
}

void Display::updateDisplay(const char* str, int num, int col){
    if (strlen(str)+6 > max_output_chars_){
        ev3_lcd_draw_string("message to output is too long", 0, font_height_ * col);
    }

    sprintf(output_str_buffer_, "%s : %6d", str, num);
    ev3_lcd_draw_string(output_str_buffer_, 0, font_height_ * col);
}
