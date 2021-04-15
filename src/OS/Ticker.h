#ifndef TICKER_H
#define TICKER_H

/**
 * @brief A Ticker will execute a Callback as long as it is not stopped in a given intervall
 * 
 */
class Ticker : public ScheduledCallable {
    public:
        Ticker() {}
        ~Ticker() {
            detach();
        }

        /**
         * @brief Attach a callback to the Ticker which should be executed each x seconds
         * 
         * @param callback 
         * @param time The time after the callback should be called repeatedly
         */
        void attach(Callback<void> callback, float time) {
            _callback = callback;
            setSleeptime(time);
            resetSleepTimer();
            scheduler.addScheduled(*this);
        }

        /**
         * @brief Detach the Ticker. Will stop execution of the callback
         * 
         */
        void detach() {
            scheduler.removeScheduled(*this);
        }

        /**
         * @brief Explicitly call the callback
         * 
         */
        void call() {
            _callback.call();
        }
    
    private:
        Callback<void> _callback;
};

#endif // TICKER_H