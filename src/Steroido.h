#ifndef STEROIDO_H
#define STEROIDO_H

#include "Common/memCpy.h"
#include "Common/memSet.h"
#include "Common/mapC.h"

// Define a standard wait time, e.g. for a loop wait
#define STEROIDO_STD_WAIT_TIME 0.0001 // s


// Include Arduino Hardware Drivers
#ifdef Arduino_h
    // Baseclass
    #include "Common/NonCopyable.h"

    // STL
    #ifdef TEENSY
        #include <vector>
        #define printf Serial.printf
    #else
        #include "Common/vector.h"
        #include "AbstractionLayer/Arduino/printfIntegration.h"
    #endif

    // Abstraction Layer
    #include "Common/Callback.h"
    #include "Common/CircularBuffer.h"
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

    #define STEROIDO_SCHEDULER_RUN_NEEDED
    Scheduler scheduler;

    #include "OS/Ticker.h"

    

    #define wait(seconds) delay(seconds * 1000)
#endif // Arduino_h


#ifdef TEENSY
    #include "AbstractionLayer/Teensyduino/CAN.h"

    // Show that CAN is possible
    #define STEROIDO_DEVICE_CAN
#endif // TEENSY


#ifdef MBED_H
    #include <vector>
    #define VECTOR_EMPLACE_BACK_ENABLED

    #include "platform/CircularBuffer.h"

    #ifdef DEVICE_CAN
        #define STEROIDO_DEVICE_CAN
        #define STEROIDO_CAN_INTERRUPT_ENABLED
    #endif

    #define STEROIDO_WAIT_NEEDED
    #define STEROIDO_INTERRUPT_IN_ENABLED

    #include "Common/setupLoopWrapper.h"
#endif // MBED_H


#include "Common/DelayedSwitch.h"

// Auto-Run in a loop
#ifdef STEROIDO_CONSUME_LOOP
    #ifndef STEROIDO_DISABLE_LOOP
        void loop() {
            Scheduler::run();
            #ifdef STEROIDO_WAIT_NEEDED
                wait(STEROIDO_STD_WAIT_TIME);
            #endif
        }
    #endif
#endif

#endif // STEROIDO_H