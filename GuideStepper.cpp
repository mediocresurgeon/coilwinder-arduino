#include "GuideStepper.h"
#include "TimerOne.h"

#define INTERRUPT_PIN 20

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


static GuideStepper* GuideStepper::getInstance(uint8_t calibrationInterruptPin, uint8_t enablePin1, uint8_t enablePin2, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
    if (0 == s_guideStepper) {
        s_guideStepper = new GuideStepper(calibrationInterruptPin, enablePin1, enablePin2, pin1, pin2, pin3, pin4);
    }
    return s_guideStepper;
}


GuideStepper::GuideStepper(uint8_t calibrationInterruptPin, uint8_t enablePin1, uint8_t enablePin2, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
    : Nema17(pin1, pin2, pin3, pin4),
      m_currentSteps(0),
      m_targetSteps(0),
      m_interruptPin(calibrationInterruptPin),
      m_enablePin1(enablePin1),
      m_enablePin2(enablePin2) {
        pinMode(m_interruptPin, INPUT_PULLUP);
        pinMode(m_enablePin1, OUTPUT);
        pinMode(m_enablePin2, OUTPUT);
        auto _SREG = SREG;  // save the current (interrupt) state
        cli();              // stop interrupts from firing
        pinMode(m_interruptPin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(m_interruptPin), onSwitch, CHANGE);
        Timer1.initialize(MAX_PACE);
        SREG = _SREG;       // restore (interrupt) state
        
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


void GuideStepper::powerOn() {
    digitalWrite(m_enablePin1, HIGH);
    digitalWrite(m_enablePin2, HIGH);
    Nema17::powerOn();
    calibrate();
}

void GuideStepper::powerOff() {
    digitalWrite(m_enablePin1, LOW);
    digitalWrite(m_enablePin2, LOW);
    Nema17::powerOff();
}


void GuideStepper::calibrate() {
    setDirection(Anticlockwise);
    s_switchPushed = false;
    while (!s_switchPushed) {
        moveHalfStep();
        delayMicroseconds(MAX_PACE);
    }
    setDirection(Clockwise);
    for (uint32_t i = 0; i < HALFSTEPS_TO_CALIBRATE; i++) {
        moveHalfStep();
        delayMicroseconds(MAX_PACE);
    }
}