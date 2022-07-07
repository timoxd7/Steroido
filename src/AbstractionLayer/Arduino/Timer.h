#ifndef TIMER_H
#define TIMER_H

#include "Common/ITimer.h"

#ifndef NUCLEO
/**
 * @brief Easy way to measure Time
 * 
 */
class Timer : public ITimer {
    public:
        Timer() : ITimer() {}
        Timer(bool autoStart) : ITimer(autoStart) {}
    
    private:
        unsigned long getMillis() {
            return millis();
        }
};
#else
typedef ITimer Timer;
#endif

#endif // TIMER_H
