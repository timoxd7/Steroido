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
         * @brief Start the Timer if not already running
         * 
         */
        void start() {
            if (!_running) {
                if (_stopped) {
                    _startedAt = getMillis() - (_stoppedAt - _startedAt);
                    _stopped = false;
                } else {
                    _startedAt = getMillis();
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
         * @brief Read the value of the Timer
         * 
         * @return float Seconds running
         */
        float read() {
            return (float)read_ms() / 1000.0;
        }

        /**
         * @brief Read the value of the Timer
         * 
         * @return unsigned long Milliseconds running
         */
        unsigned long read_ms() {
            unsigned long returnValue;
            
            if(_running) {
                if (_stopped) {
                    returnValue = _stoppedAt - _startedAt;
                } else {
                    returnValue = getMillis() - _startedAt;
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