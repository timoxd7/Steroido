#ifndef DELAYED_SWITCH_H
#define DELAYED_SWITCH_H


/*
    A simple Switch to filter out noise of a digital signal
    (e.g. for a button)
*/

#ifndef delayed_switch_time_t
typedef uint16_t delayed_switch_time_t;
#endif

class DelayedSwitch : private NonCopyable<DelayedSwitch> {
    public:
        /*
            Construct a Delayed Switch with a predefined value

            @param state Predefine a state at the start of the Delayed Switch. By Standard, it is false.
        */
        DelayedSwitch(bool state = false) : _currentState(state) {}

        /*
            Get the current State of the Switch

            Note: set(bool) should be called before

            @return bool current State of the Switch
        */
        bool get() {
            return _currentState;
        }

        /*
            Shorthand for get()
        */
        operator bool() {
            return get();
        }

        /*
            Set the current State of the Signal behind the Switch
            
            @param state bool current State of the digital Signal watched by the Switch
            @return bool the current state of the switch (same as get())
        */
        bool set(bool state) {
            if (state == _currentState) {
                // -> Signal is same than last time
                if (_changeOccured) {
                    // -> But has changed (can be noise)
                    if (_currentState) {
                        // -> Enable or Disable? Different timings for that
                        if (_transitionTimer.read_ms() >= _disableTime) {
                            // -> Change occured but signal got fastly back to old state
                            // -> Reset change variable, leave current state the same
                            _changeOccured = false;
                        }
                    } else {
                        if (_transitionTimer.read_ms() >= _enableTime) {
                            // Same as above
                            _changeOccured = false;
                        }
                    }
                }
            } else {
                // -> Signal is different than the last stable state
                if (_changeOccured) {
                    // -> Change occured before, check if it was long enough in the past
                    if (_currentState) {
                        // -> Enable or Disable? Different timings for that
                        if (_transitionTimer.read_ms() >= _disableTime) {
                            // -> Change was long enough in the past, change is confirmed
                            // -> Apply new state, reset change variable
                            _currentState = false;
                            _changeOccured = false;
                        }
                    } else {
                        if (_transitionTimer.read_ms() >= _enableTime) {
                            // Same as above
                            _currentState = true;
                            _changeOccured = false;
                        }
                    }
                } else {
                    // -> Change occured for the first time at this timeframe
                    if (_currentState) {
                        // -> Again, Enable or Disable? Different timings
                        if (_disableTime == 0) {
                            // -> If disable Time is 0, do change immideatly
                            _currentState = false;
                        } else {
                            // -> Else, flag changed signal
                            _signalChanged();
                        }
                    } else {
                        if (_enableTime == 0) {
                            // Same as above
                            _currentState = true;
                        } else {
                            _signalChanged();
                        }
                    }
                }
            }

            return get();
        }

        /*
            Shorthand for set(bool state)
        */
        DelayedSwitch &operator= (bool state) {
            set(state);
            return *this;
        }

        /*
            Set the Delay between the beginning of a high signal and a high state of the Switch

            @param time the Time between a high Signal and a high state of the Switch in milliseconds
        */
        void setEnableTime(delayed_switch_time_t time) {
            _enableTime = time;
        }

        /*
            Set the Delay between the beginning of a low signal and a low state of the Switch

            @param time the Time between a low Signal and a low state of the Switch in milliseconds
        */
        void setDisableTime(delayed_switch_time_t time) {
            _disableTime = time;
        }

        /*
            Reset the delayed Switch.
            This will reset the switch but not delete the timing settings (enable/disable time).

            @param state Predefine the State of the Delayed Switch. By standard, it is false.
        */
        void reset(bool state = false) {
            _currentState = state;
            _changeOccured = false;
        }
    
    private:
        bool _currentState;

        delayed_switch_time_t _enableTime = 0;
        delayed_switch_time_t _disableTime = 0;

        Timer _transitionTimer;
        bool _changeOccured = false;

        inline void _signalChanged() {
            _transitionTimer.reset();
            _transitionTimer.start();
            _changeOccured = true;
        }
};

#endif // DELAYED_SWITCH_H