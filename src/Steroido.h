#ifndef STEROIDO_H
#define STEROIDO_H

#include "Common/freeMemory.h"
#include "Common/memCpy.h"
#include "Common/memSet.h"


// Include Arduino Hardware Drivers
#ifdef Arduino_h
    // STL
    #include <vector>

    // Abstraction Layer
    #include "AbstractionLayer/Arduino/printfIntegration.h"
    #include "AbstractionLayer/Arduino/NonCopyable.h"
    #include "AbstractionLayer/Arduino/Callback.h"
    #include "AbstractionLayer/Arduino/CircularBuffer.h"
    #include "AbstractionLayer/Arduino/Timer.h"
    #include "AbstractionLayer/Arduino/PinName.h"
    #include "AbstractionLayer/Arduino/PinMode.h"
    #include "AbstractionLayer/Arduino/AnalogIn.h"
    #include "AbstractionLayer/Arduino/AnalogOut.h"
    #include "AbstractionLayer/Arduino/DigitalIn.h"
    #include "AbstractionLayer/Arduino/DigitalOut.h"
    #include "AbstractionLayer/Arduino/PwmOut.h"

    // OS
    #include "OS/ICallable.h"
    #include "OS/ScheduledCallable.h"
    #include "OS/Scheduler.h"
    #include "OS/Ticker.h"
#endif // Arduino_h


#ifdef TEENSY
    #include "AbstractionLayer/Teensyduino/CAN.h"
#endif // TEENSY


#ifdef MBED_H
    // Include nothing, brings all with it
    #define VECTOR_EMPLACE_BACK_ENABLED

    #ifdef DEVICE_CAN
        #define STEROIDO_DEVICE_CAN
    #endif

    #include "Common/setupLoopWrapper.h"
#endif // MBED_H


#include "Common/DelayedSwitch.h"

// Auto-Run in a loop
#ifdef STEROIDO_CONSUME_LOOP
    #ifndef STEROIDO_DISABLE_LOOP
        void loop() {
            Scheduler::run();
        }
    #endif
#endif

#endif // STEROIDO_H