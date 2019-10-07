#include "Nema17.h"
#include "Arduino.h"

Nema17::Nema17(uint8_t pin1,
               uint8_t pin2,
               uint8_t pin3,
               uint8_t pin4) 
    : m_isOn(false),
      m_pin1(pin1),
      m_pin2(pin2),
      m_pin3(pin3),
      m_pin4(pin4),
      m_direction(Clockwise),
      m_currentState(Off) {
        pinMode(m_pin1, OUTPUT);
        pinMode(m_pin2, OUTPUT);
        pinMode(m_pin3, OUTPUT);
        pinMode(m_pin4, OUTPUT);
}


Nema17::~Nema17() {
    powerOff();
}


bool Nema17::isPoweredOn() {
    return m_isOn;
}


void Nema17::powerOn() {
    m_isOn = true;
}


void Nema17::powerOff() {
    setState(Off);
    m_isOn = false;
}


void Nema17::togglePower() {
    if (isPoweredOn()) {
        powerOff();
    } else {
        powerOn();
    }
}


void Nema17::moveHalfStep() {
    setState(getNextState());
}


StepperState Nema17::getCurrentState() {
    return m_currentState;
}


void Nema17::setState(StepperState newState) {
    if (!isPoweredOn() || m_currentState == newState) {
        return;
    }
    m_currentState = newState;
    digitalWrite(m_pin1, ((m_currentState & 0x8) ? HIGH : LOW));
    digitalWrite(m_pin2, ((m_currentState & 0x4) ? HIGH : LOW));
    digitalWrite(m_pin3, ((m_currentState & 0x2) ? HIGH : LOW));
    digitalWrite(m_pin4, ((m_currentState & 0x1) ? HIGH : LOW));
}


StepperState Nema17::getNextState() {
    if (!isPoweredOn()) {
        return Off;
    }
    if (Clockwise == getDirection()) {
        switch (getCurrentState()) {
            case HHLL: return HLLL;
            case LHLL: return HHLL;
            case LHHL: return LHLL;
            case LLHL: return LHHL;
            case LLHH: return LLHL;
            case LLLH: return LLHH;
            case HLLH: return LLLH;
            case HLLL: return HLLH;
            default:   return HLLL;
        }
    } else { // Anticlockwise
        switch (getCurrentState()) {
            case HLLL: return HHLL;
            case HHLL: return LHLL;
            case LHLL: return LHHL;
            case LHHL: return LLHL;
            case LLHL: return LLHH;
            case LLHH: return LLLH;
            case LLLH: return HLLH;
            case HLLH: return HLLL;
            default:   return HLLL;
        }
    }
}


Direction Nema17::getDirection() {
    return m_direction;
}


void Nema17::setDirection(Direction newDirection) {
    m_direction = newDirection;
}


void Nema17::toggleDirection() {
    if (Clockwise == getDirection()) {
        setDirection(Anticlockwise);
    } else {
        setDirection(Clockwise);
    }
}