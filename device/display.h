/**
 * @file display.h
 * @brief ディプレイ表示用クラス
 * @author 兎澤 佑
 * @date 2015/07/10
 */

#ifndef DISPLAY_
#define DISPLAY_

#include "ev3api.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

class Display
{
private:
    static Display* instance_;    //!< 共有インスタンス

    // フォントサイズ
    int32_t font_width_;          //!< フォント幅
    int32_t font_height_;         //!< フォント高

    uint8_t max_output_chars_;    //!< 出力可能文字数
    char* output_str_buffer_;     //!< 出力文字列のバッファ

    Display();                    //!< コンストラクタ

public:
    /**
     * @brief インスタンスの取得
     * @details シングルトンパターンに基づいて共有インスタンスを取得する
     * @return 共有インスタンス
     * @author 兎澤 佑
     */
    static Display* getInstance();

    /**
     * @brief 出力フォントを設定する
     * @param font_type 出力フォント(EV3_FONT_SMALL, EV3_FONT_MIDIUM)
     * @author 兎澤 佑
     */
    void setFont(lcdfont_t font_type);

    /**
     * @brief LCDをクリアーする
     * @details LCD画面を白で塗りつぶす
     * @author 兎澤 佑
     */
    void clearDisplay();

    /**
     * @brief 文字列の出力
     * @details 引数の文字列を指定行に出力する
     *
     * @param str 出力文字列
     * @param col 出力行
     * @author 兎澤 佑
     */
    void updateDisplay(const char* str, int col);

    /**
     * @brief 文字列及び数値の出力
     * @details 「文字列：数値」の形式で，指定行に出力する
     *
     * @param str 出力文字列
     * @param num 出力数値
     * @param col 出力行
     * @author 兎澤 佑
     */
    void updateDisplay(const char* str, int num, int col);
};

#endif
