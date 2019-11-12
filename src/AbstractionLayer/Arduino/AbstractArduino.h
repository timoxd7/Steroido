#ifndef ABSTRACT_ARDUINO_H
#define ABSTRACT_ARDUINO_H

// Pow for a given type
template<typename T>
T powC(T base, T exponent) {
    if (exponent == 0) return 1;

    for (T i = 1; i < exponent; i++) {
        base *= base;
    }

    return base;
}

#include "PinName.h"
#include "Common/NonCopyable.h"
#include "AnalogIn.h"
#include "AnalogOut.h"
#include "DigitalIn.h"
#include "DigitalOut.h"
#include "PwmOut.h"
#include "Timer.h"

// Wait for a given amout of time (busy loop)
void wait(float s) {
    Timer timer;
    timer.start();
    while(timer < s);
}

#endif // ABSTRACT_ARDUINO_H