#ifndef SCHEDULED_CALLABLE_H
#define SCHEDULED_CALLABLE_H

#include "ICallable.h"

typedef float sleeptime_t;

/**
 * @brief A Callable which can be called called with a given schedule
 * 
 */
class ScheduledCallable : public ICallable {
    public:
        ScheduledCallable() : _sleeptime(0) {}
        ScheduledCallable(sleeptime_t sleeptime) : _sleeptime(sleeptime) {}

        sleeptime_t getSleepingSince() {
            return _sleepingSince;
        }

        void setSleeptime(sleeptime_t sleeptime) {
            _sleeptime = sleeptime;
        }

        sleeptime_t getSleepTime() {
            return _sleeptime;
        }

        void resetSleepTimer() {
            _sleepingSince.reset();
            _sleepingSince.start();
        }
    
    private:
        Timer _sleepingSince;
        sleeptime_t _sleeptime;
};

#endif