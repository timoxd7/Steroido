#ifndef TIMER_H
#define TIMER_H

#include "Common/ITimer.h"

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

#endif // TIMER_H
