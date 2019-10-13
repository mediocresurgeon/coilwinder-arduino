#ifndef _GUIDESTEPPER_H
#define _GUIDESTEPPER_H


#include "Nema17.h"


class GuideStepper : public Nema17 {
    public:
        // static methods
        static GuideStepper* getInstance(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
        static void GuideStepper::onTimerTick();

        // override methods
        virtual void powerOn();

        // methods
        void runUntil(uint16_t);


    private:
        // constructor
        GuideStepper(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

        // methods
        void calibrate();
        void smartStep();

        // fields
        const uint8_t m_interruptPin;
        uint16_t m_currentSteps;
        uint16_t m_targetSteps;
        uint16_t m_position;
};

#endif