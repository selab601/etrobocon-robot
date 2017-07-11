#include "Button.h"

namespace device
{
    // インスタンス変数初期化
    Button* Button::instance_ = 0;

    Button* Button::getInstance(){
        if(!instance_){
            instance_ = new Button();
        }
        return instance_;
    }

    Button::Button(){}

    bool Button::isClicked(button_t button){
        bool isPressed = ev3_button_is_pressed(button);
        if (hasPressed_[button] && !isPressed){
            hasPressed_[button] = isPressed;
            return true;
        }
        hasPressed_[button] = isPressed;
        return false;
    }

    bool Button::leftClicked()  {return isClicked(LEFT_BUTTON);}
    bool Button::rightClicked() {return isClicked(RIGHT_BUTTON);}
    bool Button::upClicked()    {return isClicked(UP_BUTTON);}
    bool Button::downClicked()  {return isClicked(DOWN_BUTTON);}
    bool Button::enterClicked() {return isClicked(ENTER_BUTTON);}
    bool Button::backClicked()  {return isClicked(BACK_BUTTON);}

}

