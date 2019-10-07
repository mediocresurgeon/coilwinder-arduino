#ifndef NEMA17_H
#define NEMA17_H

#include <stdint.h>

enum Direction {
    Clockwise,
    Anticlockwise
};

enum StepperState {
    Off  = 0x0,
    HLLL = 0x8,
    HHLL = 0xC,
    LHLL = 0x4,
    LHHL = 0x6,
    LLHL = 0x2,
    LLHH = 0x3,
    LLLH = 0x1,
    HLLH = 0x9,
};

class Nema17 {
    public:
        // constructor
        Nema17(uint8_t, uint8_t, uint8_t, uint8_t);
        // destructor
        virtual ~Nema17();
        
        // methods
        virtual bool isPoweredOn();
        virtual void powerOn();
        virtual void powerOff();
        virtual void togglePower();
        
        virtual void moveHalfStep();

    protected:
        // methods
        virtual StepperState getCurrentState();
        virtual void setState(StepperState);
        virtual StepperState getNextState();
        virtual Direction getDirection();
        virtual void setDirection(Direction);
        virtual void toggleDirection();

    private:
        // fields
        bool m_isOn;
        const uint8_t m_pin1;
        const uint8_t m_pin2;
        const uint8_t m_pin3;
        const uint8_t m_pin4;
        Direction m_direction;
        StepperState m_currentState;
};

#endif