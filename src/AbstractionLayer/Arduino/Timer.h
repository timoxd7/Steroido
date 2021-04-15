#ifndef TIMER_H
#define TIMER_H

#include "Common/ITimer.h"

/**
 * @brief Easy way to measure Time
 * 
 */
class Timer : public ITimer {
    private:
        inline unsigned long getMillis() {
            return millis();
        }
};

#endif // TIMER_H