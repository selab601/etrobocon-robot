#include "BlockInputUI.h"

using namespace strategy;

namespace contest_pkg{

    BlockInputUI* BlockInputUI::instance_ = 0;

    BlockInputUI::BlockInputUI(){
        display_ = device::Display::getInstance();
        buttons_ = device::Buttons::getInstance();
        blockCode_ = BlockCode::getInstance();
    }

    BlockInputUI* BlockInputUI::getInstance(){
        if (!instance_) {
            instance_ = new BlockInputUI();
        }
        return instance_;
    }

    bool BlockInputUI::input(){
        switch (state_){
            case State::INIT:
                display_->clearDisplay();
                state_ = State::INPUT;
                break;

            case State::INPUT:
                display();
                if (listenInputEvent()){
                    // BlockCodeクラスにセット
                    blockCode_->setCode(getCode());
                    display_->clearDisplay();
                    state_ = State::CONFIRM;
                }
                break;

            case State::CONFIRM:
                display_ ->updateDisplay("      -  BLOCK  CODE  -      ", TOP_MARGIN);
                sprintf(message_ , "                             ");
                for (int i = 0; i < CODE_DIGITS_NUM; i++){
                    message_[LEFT_MARGIN + i*2] = codeDigits_[i] + '0';
                }
                display_ ->updateDisplay(message_, TOP_MARGIN + 4);
                display_->updateDisplay("      Block1 ID", blockCode_->getIdBlock1(), TOP_MARGIN +5);
                display_->updateDisplay("      Block2 ID", blockCode_->getIdBlock2(), TOP_MARGIN +6);
                display_->updateDisplay("      Block3 ID", blockCode_->getIdBlock3(), TOP_MARGIN +7);
                display_->updateDisplay("      Block4 ID", blockCode_->getIdBlock4(), TOP_MARGIN +8);
                display_->updateDisplay("  PowerBlock ID", blockCode_->getIdPowerBlockCode(), TOP_MARGIN +9);
                display_->updateDisplay("      CODE", getCode(), TOP_MARGIN +10);
                display_ ->updateDisplay("   Push [ENTER] or [BACK]    ", TOP_MARGIN + 2);

                if (buttons_->enterClicked()){
                    display_->clearDisplay();
                    state_ = State::FINISHED;
                }
                if (buttons_->backClicked()){
                    display_->clearDisplay();
                    state_ = State::INPUT;
                }
                break;

            case State::FINISHED:
                return true;
        }
        return false;
    }
    int BlockInputUI::getCode(){
        int code = 0;
        for (int i = 0; i < CODE_DIGITS_NUM; i++){
            code *= 10;
            code += codeDigits_[i];
        }
        return code;
    }

    void BlockInputUI::next(){
        selected_++;
        if (CODE_DIGITS_NUM <= selected_){
            selected_ = 0;
        }
    }
    void BlockInputUI::back(){
        selected_--;
        if (0 > selected_){
            selected_ = CODE_DIGITS_NUM - 1;
        }
    }
    void BlockInputUI::increment(){
        codeDigits_[selected_]++;
        if (9 < codeDigits_[selected_]){
            codeDigits_[selected_] = 0;
        }
    }
    void BlockInputUI::decrement(){
        codeDigits_[selected_]--;
        if (0 > codeDigits_[selected_]){
            codeDigits_[selected_] = 9;
        }
    }

    void BlockInputUI::display(){
        display_->updateDisplay("     - INPUT BLOCK CODE -    ", TOP_MARGIN);

        sprintf(message_, "                             ");
        message_[LEFT_MARGIN + selected_ *2] = '#';
        display_->updateDisplay(message_, TOP_MARGIN + 3);
        display_->updateDisplay(message_, TOP_MARGIN + 5);

        for (int i = 0; i < CODE_DIGITS_NUM; i++){
            message_[LEFT_MARGIN + i*2] = codeDigits_[i] + '0';
        }
        message_[LEFT_MARGIN + selected_*2 -1] = '<';
        message_[LEFT_MARGIN + selected_*2 +1] = '>';
        display_->updateDisplay(message_, TOP_MARGIN +4);

        display_->updateDisplay("   Push [ENTER] to Finish    ", TOP_MARGIN +7);
    }
    bool BlockInputUI::listenInputEvent(){
        if (buttons_->leftClicked()){
            back();
        }
        if (buttons_->rightClicked()){
            next();
        }
        if (buttons_->upClicked()){
            increment();
        }
        if (buttons_->downClicked()){
            decrement();
        }
        if (buttons_->enterClicked()){
            return true;
        }
        return false;
    }

}
