#ifndef BUTTON_H_
#define BUTTON_H_

#include "ev3api.h"

namespace device
{
    class Buttons
    {
    private:
        //! 共有インスタンス
        static Buttons* instance_;
        bool hasPressed_[6] = { false, false, false,
                                false, false, false};

    private:
        Buttons();

    public:
        static Buttons* getInstance();

        /**
         * @brief クリックされたときtrueを返す
         *
         * @param button 調べるボタン
         *
         * @return クリックされたときtrue
         */
        bool isClicked(button_t button);

        /**
         * @brief クリックされたときtrueを返す
         *
         * @return クリックされたときtrue
         */
        bool leftClicked();
        bool rightClicked();
        bool upClicked();
        bool downClicked();
        bool enterClicked();
        bool backClicked();

    };
}

#endif
