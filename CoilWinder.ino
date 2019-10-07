#include "MCUFRIEND_kbv.h"
#include "Chihai.h"
#include "CoilWinderGui.h"
#include "GuideStepper.h"

// Display screen constants
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Touchscreen constants
#define MIN_PRESSURE     10
#define MAX_PRESSURE   1000
#define DEBOUNCE_MILLIS 100

// Guide stepper constants
#define GSI 20  // Must be an interrupt pin (18-21)
#define GSP 36  // power for calibration switch
#define GS1 23
#define GS2 25
#define GS3 27
#define GS4 29

TouchScreen ts(XP, YP, XM, YM, 300);
Chihai* chihai;
CoilWinderGui* gui;
GuideStepper* wireGuide;

// Global variables
// (used for debouncing the touchscreen).
// This lets us avoid the use of the delay() function
unsigned long lastBtnDownPressTime; // Timestamp of last valid "Down" touch
unsigned long lastBtnUpPressTime;   // Timestamp of last valid "Up" touch


#define CI 21  // Must be an interrupt pin (18-21)
#define CS 46  // Must be an analog output pin (44-46)
// We want 1 rotation every 20,000 microseconds so that nema17 can keep up
#define CHIHAI_PACE 20000


void setup() {
    Serial.begin(19200);
    
    wireGuide = GuideStepper::getInstance(GSI, GSP, GS1, GS2, GS3, GS4);
    chihai = Chihai::getInstance(CI, CS);
    gui = new CoilWinderGui(chihai);

    gui->start();
    chihai->setPace(CHIHAI_PACE);

    wireGuide->powerOn();
}


void loop() { 
    unsigned long now = millis(); // A timestamp for this loop

    /////////////////////////////////////////
    // Begin handling touchscreen commands //
    /////////////////////////////////////////
    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT); // IF YOU DON'T CALL THESE
    pinMode(YP, OUTPUT); // THE SCREEN WILL BE FROZEN

    if (p.z > MIN_PRESSURE && p.z < MAX_PRESSURE) {
        TouchArea area = gui->getTouchArea(&p);
        switch (area) {
            case Up:
                if (now > (lastBtnUpPressTime + DEBOUNCE_MILLIS)) { 
                    gui->onUpBtnPress();
                    lastBtnUpPressTime = now;
                }
                break;
            case Down:
                if (now > (lastBtnDownPressTime + DEBOUNCE_MILLIS)) {
                    gui->onDownBtnPress();
                    lastBtnDownPressTime = now;
                }
                break;
            case Okay:
                gui->onOkayBtnPress();
                break;
            case Cancel:
                gui->onCancelBtnPress();
                break;
            case Coils:
                gui->onCoilsFieldPress();
                break;
            case Rotations:
                gui->onRotationsFieldPress();
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