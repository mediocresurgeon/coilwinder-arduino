#include "MCUFRIEND_kbv.h"
#include "CoilWinderGui.h"
#include "BobbinDriver.h"

// Display screen constants
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Touchscreen constants
#define MIN_PRESSURE     10
#define MAX_PRESSURE   1000
#define DEBOUNCE_MILLIS 100

// Chihai constants
#define BOBBIN_COUNT_PIN   21
#define BOBBIN_CONTROL_PIN 50
#define CHIHAI_PPM         15

// Physical components
TouchScreen ts(XP, YP, XM, YM, 300);
BobbinDriver bobbinMotor(BOBBIN_COUNT_PIN, BOBBIN_CONTROL_PIN, CHIHAI_PPM);
CoilWinderGui gui(&bobbinMotor);

// Global variables
// (used for debouncing the touchscreen).
// This lets us avoid the use of the delay() function
unsigned long lastBtnDownPressTime; // Timestamp of last valid "Down" touch
unsigned long lastBtnUpPressTime;   // Timestamp of last valid "Up" touch


void setup() {
    Serial.begin(9600);
    bobbinMotor.init();
    gui.start();
}


void loop() { 
    unsigned long now = millis(); // A timestamp for this loop

    // Turn off motors when required number of rotations has been reached
    if (Active == bobbinMotor.getState()
        && gui.getDesiredRotations() <= bobbinMotor.getRotationCount()) {
        bobbinMotor.stop();
    }

    /////////////////////////////////////////
    // Begin handling touchscreen commands //
    /////////////////////////////////////////
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT); // IF YOU DON'T CALL THESE
    pinMode(YP, OUTPUT); // THE SCREEN WILL BE FROZEN

    if (p.z > MIN_PRESSURE && p.z < MAX_PRESSURE) {
        TouchArea area = gui.getTouchArea(&p);
        switch (area) {
            case Up:
                if (now > (lastBtnUpPressTime + DEBOUNCE_MILLIS)) { 
                    gui.onUpBtnPress();
                    lastBtnUpPressTime = now;
                }
                break;
            case Down:
                if (now > (lastBtnDownPressTime + DEBOUNCE_MILLIS)) {
                    gui.onDownBtnPress();
                    lastBtnDownPressTime = now;
                }
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
    ///////////////////////////////////////
    // End handling touchscreen commands //
    ///////////////////////////////////////
}