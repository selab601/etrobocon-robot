#ifndef BLOCK_INPUT_H_
#define BLOCK_INPUT_H_
#define CODE_DIGITS_NUM 6   // 初期位置コードの桁数
#define LEFT_MARGIN     9  // 表示するときの左側の余白
#define TOP_MARGIN      3   // 表示するときの上の余白

#include "../strategy/BlockArea/BlockCode.h"
#include "device/Display.h"
#include "device/Buttons.h"

namespace contest_pkg{
    class BlockInputUI{
        public:
            /**
             * @brief インスタンスを取得する．
             * @details シングルトンパターンに基づいて，共有のインスタンスを取得する．
             * @return インスタンス
             */
            static BlockInputUI* getInstance();

            /**
             * @brief ブロック初期位置の入力を行う
             *
             * @return 終了したらtrue
             */
            bool input();

            /**
             * @brief 入力されたコードを取得する
             *
             * @return 入力されたコードの値
             */
            int getCode();
        private:

            enum class State{
                INIT,
                INPUT,
                CONFIRM,
                FINISHED,
            };

            static BlockInputUI* instance_;	// インスタンス

            //コンストラクタ
            BlockInputUI();

            State state_ = State::INIT;

            // コードの各桁の値(10進数, 1の位:[5])
            int8_t codeDigits_[CODE_DIGITS_NUM] = {0, 0, 0, 0, 0, 0};
            int8_t selected_ = 0;

            // ディスプレイの文字列操作用
            char message_[30];

            strategy::BlockCode* blockCode_;
            device::Display* display_;
            device::Buttons* buttons_;


            void next();
            void back();
            void increment();
            void decrement();
            void display();
            bool listenInputEvent();
    };
}
#endif
