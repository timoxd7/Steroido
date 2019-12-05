#ifndef ANALOG_IN_H
#define ANALOG_IN_H

#ifndef ANALOG_IN_RESOLUTION
    #define ANALOG_IN_RESOLUTION 10 // bit
#endif

class AnalogIn : private NonCopyable<AnalogIn> {
    public:
        AnalogIn(PinName pin)
        : _pin(pin) {
            pinMode(pin, INPUT);
            _devider = (1 << ANALOG_IN_RESOLUTION) - 1;
        }

        float read() {
            return (float)analogRead(_pin) / (float)_devider;
        }

        uint16_t read_u16() {
            uint16_t value = (analogRead(_pin) << (16 - ANALOG_IN_RESOLUTION));
            value |= (value >> ANALOG_IN_RESOLUTION);

            return value;
        }

        operator float() {
            return read();
        }

    private:
        PinName _pin;
        uint16_t _devider;
};

#endif // ANALOG_IN_H