#include "Nema17.h"
#include "Arduino.h"


Nema17::Nema17(uint8_t enablePin1, uint8_t enablePin2, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
    : m_enablePin1(enablePin1),
      m_enablePin2(enablePin2),
      m_pin1(pin1),
      m_pin2(pin2),
      m_pin3(pin3),
      m_pin4(pin4),
      m_state(Off),
      m_direction(Clockwise),
      m_isOn(false) {
        pinMode(m_enablePin1, OUTPUT);
        pinMode(m_enablePin2, OUTPUT);
        pinMode(m_pin1, OUTPUT);
        pinMode(m_pin2, OUTPUT);
        pinMode(m_pin3, OUTPUT);
        pinMode(m_pin4, OUTPUT);
}


Direction Nema17::getDirection() {
    return m_direction;
}

void Nema17::setDirection(Direction direction) {
    m_direction = direction;
}


void Nema17::toggleDirection() {
    Direction newDirection = (Clockwise == getDirection()) ? Anticlockwise
                                                           : Clockwise;
    setDirection(newDirection);
}


void Nema17::togglePower() {
    if (Off == m_state) {
        powerOn();
    } else {
        powerOff();
    }
}


void Nema17::powerOn() {
    digitalWrite(m_enablePin1, HIGH);
    digitalWrite(m_enablePin2, HIGH);
    m_isOn = true;
}


void Nema17::powerOff() {
    digitalWrite(m_enablePin1, LOW);
    digitalWrite(m_enablePin2, LOW);
    m_state = Off;
    setState(m_state);
    m_isOn = false;
}


void Nema17::moveHalfStep() {
    if (!m_isOn) {
        return;
    }
    m_state = getNextState();
    setState(m_state);
}


void Nema17::setState(StepperState state) {
    digitalWrite(m_pin1, m_state & 0x8);
    digitalWrite(m_pin2, m_state & 0x4);
    digitalWrite(m_pin3, m_state & 0x2);
    digitalWrite(m_pin4, m_state & 0x1);
}


StepperState Nema17::getNextState() {
    if (Clockwise == getDirection()) {
        switch (m_state) {
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
    } else {
        switch (m_state) {
            case HLLL: return HLLH;
            case HHLL: return HLLL;
            case LHLL: return HHLL;
            case LHHL: return LHLL;
            case LLHL: return LHHL;
            case LLHH: return LLHL;
            case LLLH: return LLHH;
            case HLLH: return LLLH;
            default:   return HLLL;
        }
    }
}