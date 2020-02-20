#ifndef TESTING_HEADER_H
#define TESTING_HEADER_H

#include <unity.h>
#include "Common/NonCopyable.h"
#define STEROIDO_DISABLE_LOOP
#include "Common/ITimer.h"

#include "Common/memCpy.h"
#include "Common/memSet.h"

#ifdef USE_MBED
    #include "mbed.h"
#endif

#if defined(USE_ARDUINO) || defined(USE_TEENSY)
    #include "Arduino.h"
#else
    #include "Common/setupLoopWrapper.h"
#endif

unsigned long _millis = 0;

class Timer : public ITimer {
    private:
        virtual unsigned long getMillis() {
            return _millis;
        }
};

#endif // TESTING_HEADER_H