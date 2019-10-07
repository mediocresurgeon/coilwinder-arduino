#include "GuideStepper.h"
#include "TimerOne.h"

#define HALFSTEPS_TO_CALIBRATE 1200
#define HALFSTEPS_PER_LOOP      850
#define MAX_PACE                400 // in microseconds


static volatile bool s_switchPushed = false;


static void onSwitch() {
    s_switchPushed = true;
}


static GuideStepper* s_guideStepper = 0;


static void GuideStepper::onTimerTick() {
    if (0 == s_guideStepper->m_targetSteps || s_guideStepper->m_currentSteps < s_guideStepper->m_targetSteps) {
        s_guideStepper->moveHalfStep();
    } else {
        Timer1.stop();
    }
}


static GuideStepper* GuideStepper::getInstance(uint8_t switchPowerPin, uint8_t switchSignalPin, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
    if (0 == s_guideStepper) {
        s_guideStepper = new GuideStepper(switchPowerPin, switchSignalPin, pin1, pin2, pin3, pin4);
    }
    return s_guideStepper;
}


GuideStepper::GuideStepper(uint8_t switchPowerPin, uint8_t switchSignalPin, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
    : Nema17(pin1, pin2, pin3, pin4),
      m_currentSteps(0),
      m_targetSteps(0),
      m_switchPowerPin(switchPowerPin),
      m_switchSignalPin(switchSignalPin) {
        pinMode(m_switchPowerPin, OUTPUT);
        pinMode(m_switchSignalPin, INPUT_PULLUP);
        noInterrupts();
        attachInterrupt(digitalPinToInterrupt(m_switchSignalPin), onSwitch, CHANGE);
        interrupts();
        Timer1.initialize(MAX_PACE);
        
}


void GuideStepper::runUntil(uint16_t steps) {
    m_currentSteps = 0;
    m_targetSteps = steps;
    Timer1.attachInterrupt(GuideStepper::onTimerTick);
    Timer1.restart();
}


void GuideStepper::moveHalfStep() {
    Nema17::moveHalfStep();
    m_currentSteps++;
}


void GuideStepper::calibrate() {
    setDirection(Anticlockwise);
    digitalWrite(m_switchPowerPin, HIGH);
    s_switchPushed = false;
    while (!s_switchPushed) {
        moveHalfStep();
        delayMicroseconds(MAX_PACE);
    }
    digitalWrite(m_switchPowerPin, LOW);

    setDirection(Clockwise);
    for (uint32_t i = 0; i < HALFSTEPS_TO_CALIBRATE; i++) {
        moveHalfStep();
        delayMicroseconds(MAX_PACE);
    }
}

/*
TODO:

Add function to re-home after the Chihai is done
Modify runUntil so it toggles direction after a certain number of steps
Turn off Nema17 when it is not in operation (to keep it cool)
*/