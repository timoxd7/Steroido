#ifndef ANALOG_IN_H
#define ANALOG_IN_H

#ifndef ANALOG_IN_RESOLUTION
    #define ANALOG_IN_RESOLUTION 10 // bit
#endif

/**
 * @brief Analog Input
 * 
 */
class AnalogIn : private NonCopyable<AnalogIn> {
    public:
        /**
         * @brief Construct a new Analog In object
         * 
         * @param pin Pin this AnalogIn is assigned to
         */
        AnalogIn(PinName pin)
        : _pin(pin) {
            pinMode(pin, INPUT);
            _devider = (1 << ANALOG_IN_RESOLUTION) - 1;
        }

        /**
         * @brief Read the Pin
         * 
         * @return float Analog Value from 0.0 to 1.0
         */
        float read() {
            return (float)analogRead(_pin) / (float)_devider;
        }

        /**
         * @brief Read the Pin. More optimized for AVR boards (no float involved). Take the analog resolution in account!
         * 
         * @return uint16_t Analog Value from 0 to 65535
         */
        uint16_t read_u16() {
            uint16_t value = (analogRead(_pin) << (16 - ANALOG_IN_RESOLUTION));
            value |= (value >> ANALOG_IN_RESOLUTION);

            return value;
        }

        /**
         * @brief Optimized read for arduino boards with 10 bit resolution. Only available for Arduino framwork
         * 
         * @return uint16_t Analog Value from 0 to 1023
         */
        uint16_t read_analog_arduino() {
            return analogRead(_pin);
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
        uint16_t _devider;
};

#endif // ANALOG_IN_H