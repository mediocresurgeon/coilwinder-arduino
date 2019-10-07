#ifndef _NEMA17_H
#define _NEMA17_H

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

        // method
        virtual void moveHalfStep();
        void setDirection(Direction);
        void toggleDirection();
        void togglePower();
        void powerOn();
        void powerOff();

    private:
        // method
        StepperState getNextState();
        void setState(StepperState);

        //fields
        const uint8_t m_pin1;
        const uint8_t m_pin2;
        const uint8_t m_pin3;
        const uint8_t m_pin4;
        StepperState m_state;
        Direction m_direction;
        bool m_isOn;
};
#endif