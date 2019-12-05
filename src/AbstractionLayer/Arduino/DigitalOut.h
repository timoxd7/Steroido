#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

class DigitalOut {
    public:
        DigitalOut(PinName pin)
        : _pin(pin) {
            pinMode(pin, OUTPUT);
        }

        DigitalOut(PinName pin, uint8_t value)
        : DigitalOut(pin) {
            write(value);
        }

        /*
            Write to the Output Port. Everything greater then 1 will be
            seen as HIGH, everything below as LOW
        */
        void write(uint8_t value) {
            if (value >= 1) {
                digitalWrite(_pin, HIGH);
            } else {
                digitalWrite(_pin, LOW);
            }

            _value = value;
        }

        uint8_t read() {
            return _value;
        }

        DigitalOut &operator= (uint8_t value) {
            write(value);
            return *this;
        }

        operator uint8_t() {
            return read();
        }
    
    private:
        PinName _pin;
        uint8_t _value = 0;
};

#endif // DIGITAL_OUT_H