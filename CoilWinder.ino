#include "MCUFRIEND_kbv.h"
#include "CoilWinderGui.h"

// These are the pins that the Arduino uses to communicate
// with the touchscreen.  If these pins change, you will 
// need to update these values!
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

CoilWinderGui gui;
TouchScreen ts(XP, YP, XM, YM, 300);

void setup() {
    Serial.begin(9600);
    gui.start();
}

void loop() { 
    digitalWrite(13, HIGH);
    TSPoint p = ts.getPoint();
    digitalWrite(13, LOW);
    // Unfortunate, but the touch feature of the screen uses
    // the same XM and YP pins that the LCD screen feature uses.
    // So after calling getPoint(), we need to
    // reset those pins so that the LCDs can be updated.
    pinMode(XM, OUTPUT); // IF YOU DON'T CALL THESE
    pinMode(YP, OUTPUT); // THE SCREEN WILL BE FROZEN
    
    #define MIN_PRESSURE 10
    #define MAX_PRESSURE 1000

    if (p.z > MIN_PRESSURE && p.z < MAX_PRESSURE) {
        TouchArea area = gui.getTouchArea(&p);
        switch (area) {
            case Up:
                gui.onUpBtnPress();
                delay(100); // UI debouncing
                break;
            case Down:
                gui.onDownBtnPress();
                delay(100); // UI debouncing
                break;
            case Okay:
                gui.onOkayBtnPress();
                break;
            case Cancel:
                gui.onCancelBtnPress();
                break;
            case Coils:
                gui.onCoilsFieldPress();
                break;
            case Rotations:
                gui.onRotationsFieldPress();
                break;
            default:
                // Nothing was touched, so take no action
                break;
        }
    }
}