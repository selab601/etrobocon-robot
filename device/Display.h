#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "ev3api.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>

namespace device
{
    class Display
    {
    private:
        //! 共有インスタンス
        static Display* instance_;

        // フォントサイズ
        int32_t fontWidth_;
        int32_t fontHeight_;

        //! 出力可能文字数
        uint8_t maxOutputChars_;    //!< 出力可能文字数
        //! 出力文字列のバッファ
        char* outputStrBuffer_;

    private:
        Display();

    public:
        static Display* getInstance();

        /**
         * @brief 出力フォントを設定する
         * @param font_type 出力フォント(EV3_FONT_SMALL, EV3_FONT_MIDIUM)
         */
        void setFont(lcdfont_t fontType);

        /**
         * @brief LCDをクリアーする
         * @details LCD画面を白で塗りつぶす
         */
        void clearDisplay();

        /**
         * @brief 文字列の出力
         * @details 引数の文字列を指定行に出力する
         *
         * @param str 出力文字列
         * @param col 出力行
         */
        void updateDisplay(const char* str, int col);

        /**
         * @brief 文字列及び数値の出力
         * @details 「文字列：数値」の形式で，指定行に出力する
         *
         * @param str 出力文字列
         * @param num 出力数値
         * @param col 出力行
         */
        void updateDisplay(const char* str, int num, int col);
    };
}

#endif
