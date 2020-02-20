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
        /**
        * @brief Construct a Delayed Switch with a predefined value
        * 
        * @param state Predefine a state at the start of the Delayed Switch. By Standard, it is false.
        */
        DelayedSwitch(bool state = false) : _currentState(state), _lastSetState(state) {}

        /**
        * @brief Get the current State of the Switch
        * 
        * @return true current State of the Switch
        * @return false current State of the Switch
        */
        bool get() {
            return set(_lastSetState);
        }

        /**
         * @brief Shorthand for get()
         * 
         * @return true 
         * @return false 
         */
        operator bool() {
            return get();
        }

        /**
         * @brief Set the current State of the Signal behind the Switch
         * 
         * @param state current State of the digital Signal watched by the Switch
         * @return true the current state of the switch
         * @return false the current state of the switch
         */
        bool set(bool state) {
            _lastSetState = state;
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

            return _currentState;
        }

        /**
         * @brief Shorthand for set(bool state)
         * 
         * @param state 
         * @return DelayedSwitch& 
         */
        DelayedSwitch &operator= (bool state) {
            set(state);
            return *this;
        }

        /**
         * @brief Set the Delay between the beginning of a high signal and a high state of the Switch
         * 
         * @param time the Time between a high Signal and a high state of the Switch in milliseconds
         */
        void setEnableTime(delayed_switch_time_t time) {
            _enableTime = time;
        }

        /**
         * @brief Set the Delay between the beginning of a low signal and a low state of the Switch
         * 
         * @param time the Time between a low Signal and a low state of the Switch in milliseconds
         */
        void setDisableTime(delayed_switch_time_t time) {
            _disableTime = time;
        }

        /**
         * @brief Reset the delayed Switch. This will reset the switch but not delete the timing settings (enable/disable time).
         * 
         * @param state Predefine the State of the Delayed Switch. By standard, it is false.
         */
        void reset(bool state = false) {
            _currentState = state;
            _changeOccured = false;
        }
    
    private:
        bool _currentState;
        bool _lastSetState;

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