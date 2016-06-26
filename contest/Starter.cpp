/**
 * @file Startert.cpp
 * @date 2015/07/15
 * @author motoki nagaoka
 *
 */
#include "Starter.h"

namespace contest{
    Starter* Starter::instance = 0;

    Starter::Starter(){
        touch = device::TouchSensor::getInstance();
        display = device::Display::getInstance();

   }

    Starter* Starter::getInstance(){

       if ( ! instance ) {
           instance = new Starter();
        }
        return new Starter();
    }

    // スタートを受け付ける
bool Starter::start(){
    static bool started = false;
    static bool hasPressed = true;
    if  ( !touch->isPressed() )
        hasPressed = false;

    if ( !started && !hasPressed && touch->isPressed()){
        started = true;

        display-> updateDisplay("                            ",  0);
        display-> updateDisplay ("                            ", 1);
        display-> updateDisplay ("                            ", 2);
        display-> updateDisplay ("         S T A R T          ", 3);
    }
    else if ( !started ){
        display-> updateDisplay("                            ",  0);
        display-> updateDisplay ("                            ", 1);
        display-> updateDisplay ("                            ", 2);
        display-> updateDisplay ("         R E A D Y          ", 3);
    }
    return started;
}

}
