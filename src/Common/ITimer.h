#ifndef ITIMER_H
#define ITIMER_H

class ITimer : private NonCopyable<ITimer> {
    public:
        ITimer() {
            reset();
        }

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

        void stop() {
            if (_running && !_stopped) {
                _stoppedAt = getMillis();
                _stopped = true;
            }
        }

        void reset() {
            _startedAt = 0;
            _stoppedAt = 0;
            _running = false;
            _stopped = false;
        }

        float read() {
            return (float)read_ms() / 1000.0;
        }

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

        operator float() {
            return read();
        }
    
    private:
        unsigned long _startedAt;
        unsigned long _stoppedAt;
        bool _running;
        bool _stopped;

        virtual unsigned long getMillis() = 0;
};

#endif // ITIMER_H