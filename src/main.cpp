#include <Arduino.h>
#include "keyboard.h"
#include <ArduinoLog.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);
    pinMode(24, OUTPUT); //
    pinMode(25, OUTPUT); // Feed 74HC138 for 8 O/P each side (not gated!)
    pinMode(26, OUTPUT); //

    pinMode(27, INPUT_PULLUP); // Outside row of RHS switches
    pinMode(28, INPUT_PULLUP); // Middle row of RHS switches
    pinMode(29, INPUT_PULLUP); // Inside row of RHS switches

    pinMode(30, INPUT_PULLUP); // Outside row of LHS switches
    pinMode(31, INPUT_PULLUP); // Middle row LHS switches
    pinMode(32, INPUT_PULLUP); // Inside row of LHS switches

    // Keyboard diagnostic line
    pinMode(CKeyboard::SCAN_IN_PROGRESS_DIAGNOSTIC_LINE, OUTPUT);
    digitalWrite(CKeyboard::SCAN_IN_PROGRESS_DIAGNOSTIC_LINE, 0);
}

void loop() 
{
    CKeyboard::getInstance()->scan();
}
