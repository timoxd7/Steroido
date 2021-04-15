#ifndef DIGITAL_OUT_H
#define DIGITAL_OUT_H

/**
 * @brief Digital Output
 * 
 */
class DigitalOut {
    public:
        /**
         * @brief Construct a new Digital Out object
         * 
         * @param pin The Pin this DigitalOut is assigned to
         */
        DigitalOut(PinName pin)
        : _pin(pin) {
            pinMode(pin, OUTPUT);
        }

        /**
         * @brief Construct a new Digital Out object
         * 
         * @param pin The Pin this DigitalOut is assigned to
         * @param value The initial Value of the Digital Output
         */
        DigitalOut(PinName pin, uint8_t value)
        : DigitalOut(pin) {
            write(value);
        }

        /**
         * @brief Write to the Output. Everything greater than 1 is HIGH, everything below is LOW
         * 
         * @param value 1 for HIGH, 0 for LOW
         */
        void write(uint8_t value) {
            if (value >= 1) {
                digitalWrite(_pin, HIGH);
                _value = 1;
            } else {
                digitalWrite(_pin, LOW);
                _value = 0;
            }
        }

        /**
         * @brief Read the last set Value
         * 
         * @return uint8_t The last set Value, 1 for HIGH, 0 for LOW
         */
        uint8_t read() {
            return _value;
        }

        /**
         * @brief Shorthand for write(value)
         * 
         * @param value 1 for HIGH, 0 for LOW
         * @return DigitalOut& 
         */
        DigitalOut &operator=(uint8_t value) {
            write(value);
            return *this;
        }

        /**
         * @brief Shorthand for read()
         * 
         * @return uint8_t 1 for HIGH, 0 for LOW
         */
        operator uint8_t() {
            return read();
        }
    
    private:
        PinName _pin;
        uint8_t _value = 0;
};

#endif // DIGITAL_OUT_H