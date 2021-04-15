#ifndef ANALOG_OUT_H
#define ANALOG_OUT_H

/**
 * @brief Analog Output. For Arduino, this may only be PWM!
 * 
 */
class AnalogOut : private NonCopyable<AnalogOut> {
    public:
        /**
         * @brief Construct a new Analog Out object
         * 
         * @param pin The Pin this AnalogOut is assigned to
         */
        AnalogOut(PinName pin)
        : _pin(pin) {
            pinMode(pin, OUTPUT);
        }

        /**
         * @brief Write a Value to the analog Output. Using analogWrite under the hood. For Arduino, this can be only a PWM output!
         * 
         * @param value Value between 0 and 65535
         */
        void write_u16(uint16_t value) {
            analogWrite(_pin, (uint8_t)((uint16_t)value >> 8));
            _value = value;
        }

        /**
         * @brief Write a Value to the analog Output. Using analogWrite under the hood. For Arduino, this can be only a PWM output!
         * 
         * @param value Value between 0.0 and 1.0
         */
        void write(float value) {
            if (value > 1.0) value = 1.0;
            else if (value < 0) value = 0;

            write_u16((uint16_t)((float)value * 65535.0));
        }

        /**
         * @brief Returns the last set Value
         * 
         * @return uint16_t Last set Value 0 to 65535
         */
        uint16_t read_u16() {
            return _value;
        }

        /**
         * @brief Returns the last set Value as Float
         * 
         * @return float Last set Value 0.0 to 1.0
         */
        float read() {
            return (float)_value / 65535.0;
        }

        /**
         * @brief Shorthand for write(value)
         * 
         * @param value 
         * @return AnalogOut& 
         */
        AnalogOut &operator=(float value) {
            write(value);
            return *this;
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
        PinName _pin;
        uint16_t _value = 0;
};

#endif // ANALOG_OUT_H