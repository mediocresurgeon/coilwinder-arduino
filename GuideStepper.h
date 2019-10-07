#ifndef _GUIDESTEPPER_H
#define _GUIDESTEPPER_H


#include "Nema17.h"


class GuideStepper : public Nema17 {
    public:
        // static methods
        static GuideStepper* getInstance(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
        static void GuideStepper::onTimerTick();

        // override methods
        virtual void moveHalfStep();

        // methods
        void runUntil(uint16_t);
        void calibrate();

    private:
        // constructor
        GuideStepper(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);

        // fields
        const uint8_t m_switchPowerPin;
        const uint8_t m_switchSignalPin;
        uint16_t m_currentSteps;
        uint16_t m_targetSteps;
};

#endif