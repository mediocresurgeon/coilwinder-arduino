#include "BobbinDriver.hpp"

void onSignal() {
    signalCount++;
}

BobbinDriver::BobbinDriver(uint8_t interruptPin, uint8_t motorOnPin, uint8_t ppm) {
    m_motorOnPin = motorOnPin;
    m_interruptPin = interruptPin;
    m_ppm = ppm;
    m_state = Idle;
};

void BobbinDriver::init() {
    pinMode(m_motorOnPin, OUTPUT);
    pinMode(m_interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(m_interruptPin), onSignal, RISING);
}

void BobbinDriver::start() {
    if (m_state != Idle) return; // The motor has already been started, so exit this method
    m_state = Active;
    signalCount = 0;
    digitalWrite(m_motorOnPin, HIGH);
}

void BobbinDriver::stop() {
    if (m_state != Active) return; // The motor is already inactive, so exit this method
    m_state = Idle;
    digitalWrite(m_motorOnPin, LOW);
}

uint8_t BobbinDriver::revolutionCount() {
    return signalCount / m_ppm;
}