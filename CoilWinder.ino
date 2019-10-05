#include "MCUFRIEND_kbv.h"
#include "Chihai.h"
#include "CoilWinderGui.h"

// Display screen constants
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

// Touchscreen constants
#define MIN_PRESSURE     10
#define MAX_PRESSURE   1000
#define DEBOUNCE_MILLIS 100

TouchScreen ts(XP, YP, XM, YM, 300);
Chihai* chihai;
CoilWinderGui* gui;

// Global variables
// (used for debouncing the touchscreen).
// This lets us avoid the use of the delay() function
unsigned long lastBtnDownPressTime; // Timestamp of last valid "Down" touch
unsigned long lastBtnUpPressTime;   // Timestamp of last valid "Up" touch


#define CHIHAI_INTERRUPT_PIN 21
#define CHIHAI_SPEED_PIN     46
// We want 1 rotation every 20,000 microseconds so that nema17 can keep up
#define CHIHAI_PACE 20000


void setup() {
    Serial.begin(9600);
    
    chihai = Chihai::getInstance(CHIHAI_INTERRUPT_PIN, CHIHAI_SPEED_PIN);
    gui = new CoilWinderGui(chihai);
    
    gui->start();
    chihai->setPace(CHIHAI_PACE); 
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