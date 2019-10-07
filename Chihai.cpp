#include "Chihai.h"

// 0 -> off, 255 -> max
#define DEFAULT_SPEED 200
#define PPM            15


// Holds a reference to the singleton of Chihai
static Chihai* s_chihai = 0;

static void Chihai::onHallSignal() {
    unsigned long now = micros();
    s_chihai->m_pulseCount++;
    if (s_chihai->m_pulseCount >= s_chihai->m_targetPulseCount) {
        s_chihai->stop();
        return;
    }
    s_chihai->adjustSpeed(now);
}

void Chihai::adjustSpeed(unsigned long nowMicroSec) {
    if (m_timestamp) {
        unsigned long currentPace = nowMicroSec - m_timestamp;
        if ((currentPace > m_targetMicrosPerSignal) && (m_motorSpeedCoefficient < 255)) {
            m_motorSpeedCoefficient++;
        } else if ((currentPace < m_targetMicrosPerSignal) && (m_motorSpeedCoefficient > 0)) {
            m_motorSpeedCoefficient--;
        }
        analogWrite(m_motorSpeedPin, m_motorSpeedCoefficient);
    }
    m_timestamp = nowMicroSec;
}


Chihai::Chihai(uint8_t interruptPin, uint8_t motorSpeedPin) :
    m_motorSpeedPin(motorSpeedPin),
    m_interruptPin(interruptPin),
    m_motorSpeedCoefficient(DEFAULT_SPEED),
    m_state(Idle),
    m_targetMicrosPerSignal((500 * 40) / PPM),
    m_targetPulseCount(0),
    m_pulseCount(0),
    m_timestamp(0) {
        pinMode(m_motorSpeedPin, OUTPUT);
        pinMode(m_interruptPin, INPUT_PULLUP);
}


static Chihai* Chihai::getInstance(uint8_t interruptPin, uint8_t motorSpeedPin) {
    if (s_chihai == 0) {
        s_chihai = new Chihai(interruptPin, motorSpeedPin);
    }
    return s_chihai;
}


void Chihai::start(uint16_t rotationCount) {
    // If the Chihai is already active, don't do anything.
    if (m_state == Active) {
        return;
    }
    
    m_pulseCount = 0;
    m_timestamp = 0;
    m_targetPulseCount = rotationCount * PPM;

    m_state = Active;
    attachInterrupt(digitalPinToInterrupt(m_interruptPin), s_chihai->onHallSignal, RISING);
    analogWrite(m_motorSpeedPin, m_motorSpeedCoefficient);
}


void Chihai::stop() {
    // If the Chihai is already stopped, don't do anything.
    if (m_state == Idle) {
        return;
    }
    
    m_state = Idle;
    analogWrite(m_motorSpeedPin, LOW);
    detachInterrupt(digitalPinToInterrupt(m_interruptPin));
}

void Chihai::setPace(uint32_t microsecondsPerRotation) {
    m_targetMicrosPerSignal = microsecondsPerRotation / PPM;
}


ChihaiState Chihai::getState() {
    return m_state;
}


uint16_t Chihai::getCompletedRotations() {
    return m_pulseCount / PPM;
}