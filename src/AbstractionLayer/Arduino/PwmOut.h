#ifndef PWM_OUT_H
#define PWM_OUT_H

class PwmOut : private NonCopyable<PwmOut> {
    public:
        PwmOut(PinName pin)
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

        PwmOut &operator= (float value) {
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