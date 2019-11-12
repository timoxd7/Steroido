#ifndef ANALOG_IN_H
#define ANALOG_IN_H

#include "Common/NonCopyable.h"
#include "PinName.h"

#define ANALOG_IN_RESOLUTION 10 // bit

class AnalogIn : private NonCopyable<AnalogIn> {
    public:
        AnalogIn(PinName pin)
        : _pin(pin) {
            pinMode(pin, INPUT);
            _devider = powC<uint16_t>(2, ANALOG_IN_RESOLUTION);
        }

        float read() {
            return (float)analogRead(_pin) / (float)_devider;
        }

        uint16_t read_u16() {
            return analogRead(_pin) << ANALOG_IN_RESOLUTION;
        }

        operator float() {
            return read();
        }

    private:
        PinName _pin;
        uint16_t _devider;
};

#endif // ANALOG_IN_H