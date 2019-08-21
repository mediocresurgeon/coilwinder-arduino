
#ifndef _BOBBINDRIVER_H
#define _BOBBINDRIVER_H

volatile uint64_t signalCount;

void onSignal();

enum BobbinDriverState {
    Idle,
    Active
};

class BobbinDriver {
    public:
        // constructor
        BobbinDriver();
        BobbinDriver(uint8_t interruptPin, uint8_t motorOnPin, uint8_t ppm);

        // functions
        void init();
        void start();
        void stop();
        uint8_t revolutionCount();
        BobbinDriverState getState();

    private:
        // members
        BobbinDriverState m_state;
        uint8_t m_ppm;
        uint8_t m_motorOnPin;
        uint8_t m_interruptPin;
};

#endif
