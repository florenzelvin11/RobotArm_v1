#include "Button.h"

Button::Button(int a_init, int b_init, int c_init, int d_init) 
    : pinA{a_init}, pinB{b_init}, pinC{c_init}, pinD{d_init} {}


void Button::_init_(){
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    pinMode(pinC, INPUT_PULLUP);
    pinMode(pinD, INPUT_PULLUP);
}

bool Button::keyDown(int pinButtonRef){
    bool prevButtonState = HIGH, status = false;
    int refState = digitalRead(pinButtonRef);
    if(refState == 0){
        bool prevState = true;
        while(prevState){
            int button_state = digitalRead(pinButtonRef);
            if(button_state == HIGH && prevButtonState == LOW){
                prevButtonState = HIGH;
                status = true;
                prevState = false;
            }else{
                status = false;
                prevButtonState = button_state;
            }
            delay(250);
        }
    }
    return status;
}

int Button::keyPressed()
{
    if(keyDown(pinA)){
        return 1;
    }else if(keyDown(pinB)){
        return 2;
    }else if(keyDown(pinC)){
        return 3;
    }else if(keyDown(pinD)){
        return 4;
    }else{
        return 0;
    }
    delay(250);
}