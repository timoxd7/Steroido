#ifndef TICKER_H
#define TICKER_H

class Ticker : public ScheduledCallable {
    public:
        Ticker() {}
        ~Ticker() {
            detach();
        }

        /*
            Attach a callback to the Ticker which should be executed each x seconds

            @param time The time after the callback should be called repeadly
        */
        void attach(Callback<void> callback, float time) {
            _callback = callback;
            setSleeptime(time);
            resetSleepTimer();
            scheduler.addScheduled(*this);
        }

        /*
            Detach the Ticker
        */
        void detach() {
            scheduler.removeScheduled(*this);
        }

        void call() {
            _callback.call();
        }
    
    private:
        Callback<void> _callback;
};

#endif // TICKER_H