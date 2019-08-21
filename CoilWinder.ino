#include "MCUFRIEND_kbv.h"
#include "CoilWinderGui.h"
#include "BobbinDriver.h"

// Display screen constants
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Touchscreen constants
#define MIN_PRESSURE 10
#define MAX_PRESSURE 1000

// Chihai constants
#define BOBBIN_COUNT_PIN   21
#define BOBBIN_CONTROL_PIN 50
#define CHIHAI_PPM         15

// global variables
CoilWinderGui gui;
TouchScreen ts(XP, YP, XM, YM, 300);
BobbinDriver bobbinMotor(BOBBIN_COUNT_PIN, BOBBIN_CONTROL_PIN, CHIHAI_PPM);


void setup() {
    Serial.begin(9600);
    bobbin.init();
    gui.start();
}


void loop() { 
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT); // IF YOU DON'T CALL THESE
    pinMode(YP, OUTPUT); // THE SCREEN WILL BE FROZEN

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