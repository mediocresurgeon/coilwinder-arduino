#ifndef CHIHAI_H
#define CHIHAI_H

#include <stdint.h>

enum ChihaiState {
    Idle,
    Active
};

class Chihai {
    public:
        static Chihai* getInstance(uint8_t interruptPin, uint8_t motorSpeedPin);

        // methods
        void start(uint16_t);
        void stop();
        void setPace(uint32_t);
        ChihaiState getState();
        uint16_t getCompletedRotations();

        uint8_t m_motorSpeedCoefficient;

    private:
        // constructor
        Chihai(uint8_t, uint8_t);
        
        // methods
        static void onHallSignal();
        void adjustSpeed(unsigned long);
        
        // members
        const uint8_t m_motorSpeedPin;
        const uint8_t m_interruptPin;
        
        ChihaiState m_state;
        uint32_t m_targetMicrosPerSignal;
        uint16_t m_targetPulseCount;
        uint16_t m_pulseCount;
        unsigned long m_timestamp;
};

#endif
