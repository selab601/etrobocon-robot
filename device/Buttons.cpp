#include "Buttons.h"

namespace device
{
    // インスタンス変数初期化
    Buttons* Buttons::instance_ = 0;

    Buttons* Buttons::getInstance(){
        if(!instance_){
            instance_ = new Buttons();
        }
        return instance_;
    }

    Buttons::Buttons(){}

    bool Buttons::isClicked(button_t button){
        bool isPressed = ev3_button_is_pressed(button);
        if (hasPressed_[button] && !isPressed){
            hasPressed_[button] = isPressed;
            return true;
        }
        hasPressed_[button] = isPressed;
        return false;
    }

    bool Buttons::leftClicked()  {return isClicked(LEFT_BUTTON);}
    bool Buttons::rightClicked() {return isClicked(RIGHT_BUTTON);}
    bool Buttons::upClicked()    {return isClicked(UP_BUTTON);}
    bool Buttons::downClicked()  {return isClicked(DOWN_BUTTON);}
    bool Buttons::enterClicked() {return isClicked(ENTER_BUTTON);}
    bool Buttons::backClicked()  {return isClicked(BACK_BUTTON);}

}

