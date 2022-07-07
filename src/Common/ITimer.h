#ifndef ITIMER_H
#define ITIMER_H

/**
 * @brief Interface for the Timer. Only getMillis() has to be defined!
 * 
 */
class ITimer : private NonCopyable<ITimer> {
    public:
        /**
         * @brief Construct a new ITimer object
         * 
         */
        ITimer() {
            reset();
        }

        /**
         * @brief Construct a new ITimer with predefined state
         *
         */
        ITimer(bool autoStart) {
            reset();
            
            if (autoStart) {
                start();
            }
        }

        /**
         * @brief Start the Timer if not already running
         * 
         */
        void start() {
            start(getMillis());
        }
        
        /**
         * @brief Start the Timer if not already running, using the given currentTime
         * 
         */
        void start(unsigned long currentTime) {
            if (!_running) {
                if (_stopped) {
                    _startedAt = currentTime - (_stoppedAt - _startedAt);
                    _stopped = false;
                } else {
                    _startedAt = currentTime;
                }

                _running = true;
            }
        }

        /**
         * @brief Stop / Pause a running timer
         * 
         */
        void stop() {
            if (_running && !_stopped) {
                _stoppedAt = getMillis();
                _stopped = true;
            }
        }

        /**
         * @brief Reset the Timer back to 0 and stop it
         * 
         */
        void reset() {
            _startedAt = 0;
            _stoppedAt = 0;
            _running = false;
            _stopped = false;
        }

        /**
         * @brief Restart the Timer (reset -> start)
         *
         */
        void restart() {
            reset();
            start();
        }
    
        /**
         * @brief Restart the Timer (reset -> start)
         *
         */
        void restart(unsigned long currentTime) {
            reset();
            start(currentTime);
        }
    
        /**
         * @brief Read the value of the Timer
         *
         * @return float Seconds running
         */
        float read() { return (float)read_ms() / 1000.0; }

        /**
         * @brief Read the value of the Timer while giving the current time.
         *
         * @param currentMillis
         * @return float
         */
        float read(unsigned long currentMillis) { return (float)read_ms(currentMillis) / 1000.0; }

        /**
         * @brief Read the value of the Timer
         *
         * @return unsigned long Milliseconds running
         */
        unsigned long read_ms() { return read_ms(getMillis()); }

        /**
         * @brief Read the value of the Timer while giving the current time.
         *
         * @param currentMillis
         * @return unsigned long
         */
        unsigned long read_ms(unsigned long currentMillis) {
            unsigned long returnValue;

            if (_running) {
                if (_stopped) {
                    returnValue = _stoppedAt - _startedAt;
                } else {
                    returnValue = currentMillis - _startedAt;
                }
            } else {
                returnValue = 0;
            }

            return returnValue;
        }

        /**
         * @brief Shorthand for read()
         * 
         * @return float 
         */
        operator float() {
            return read();
        }
    
    private:
        unsigned long _startedAt;
        unsigned long _stoppedAt;
        bool _running;
        bool _stopped;

    protected:
        /**
         * @brief Has to be defined by a Timer to complete the Timers functionality
         * 
         * @return unsigned long the Milliseconds since the start of the Microcontroller
         */
        virtual unsigned long getMillis() = 0;
};

#endif // ITIMER_H
