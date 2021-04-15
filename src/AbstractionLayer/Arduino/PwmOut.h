#ifndef PWM_OUT_H
#define PWM_OUT_H

/**
 * @brief PWM Output
 * 
 */
class PwmOut : private NonCopyable<PwmOut> {
    public:
        /**
         * @brief Construct a new Pwm Out object
         * 
         * @param pin The Pin this PWM Output is assigned to
         */
        PwmOut(PinName pin)
        : _pin(pin) {
            pinMode(pin, OUTPUT);
        }

        /**
         * @brief Write a PWM value, from 0 to 65535
         * 
         * @param value PWM High Time from 0 to 65535
         */
        void write_u16(uint16_t value) {
            analogWrite(_pin, (uint8_t)((uint16_t)value >> 8));
            _value = value;
        }

        /**
         * @brief Write a PWM value, from 0.0 to 1.0
         * 
         * @param value PWM High Time from 0.0 to 1.0
         */
        void write(float value) {
            if (value > 1.0) value = 1.0;
            else if (value < 0) value = 0;

            write_u16((uint16_t)((float)value * 65535.0));
        }

        /**
         * @brief Read the last set value
         * 
         * @return float Last set value from 0.0 to 1.0
         */
        float read() {
            return (float)_value / 65535.0;
        }

        /**
         * @brief Shorthand for write(value)
         * 
         * @param value 
         * @return PwmOut& 
         */
        PwmOut &operator=(float value) {
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