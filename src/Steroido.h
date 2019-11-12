#ifndef STEROIDO_H
#define STEROIDO_H

// PreInclude Common
#include "Common/Common.h"


// Include Framework-Specific Files
#if defined(Arduino_h) && !defined(TEENSY)
    #include "AbstractionLayer/Arduino/AbstractArduino.h"
#endif // Arduino_h && !TEENSY

#if defined(Arduino_h) && defined(TEENSY)
    #include "AbstractionLayer/Teensyduino/AbstractTeensyduino.h"
#endif // Arduino_h && TEENSY

#ifdef MBED_H
    // Include nothing, brings all with it
#endif // MBED_H


#endif // STEROIDO_H