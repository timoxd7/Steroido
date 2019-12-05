#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

class AnalogOut : private NonCopyable<AnalogOut> {
    public:
        AnalogOut(PinName pin)
        : _pin(pin) {
            pinMode(pin, OUTPUT);
        }

        void write_u16(uint16_t value) {
            analogWrite(_pin, (uint8_t)((uint16_t)value >> 8));
            _value = value;
        }

        void write(float value) {
            if (value > 1.0) value = 1.0;
            else if (value < 0) value = 0;

            write_u16((uint16_t)((float)value * 65535.0));
        }

        float read() {
            return (float)_value / 65535.0;
        }

        AnalogOut &operator= (float value) {
            write(value);
            return *this;
        }

        operator float() {
            return read();
        }
    
    private:
        PinName _pin;
        uint16_t _value = 0;
};

#endif // ANALOG_OUT_H