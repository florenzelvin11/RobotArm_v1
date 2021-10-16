#pragma once
#include <Arduino.h>

class Button{
public:
    Button(int a) { this->pinA = a; }
    Button(int a_init, int b_init, int c_init, int d_init);
    void _init_();
    bool keyDown(int pinButtonRef);
    int keyPressed();
private:
    // Button pin allocation Variables
    int pinA;
    int pinB;
    int pinC;
    int pinD;
    // ##### Can Add more buttons
    
};