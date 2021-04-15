#ifndef DIGITAL_IN_H
#define DIGITAL_IN_H

/**
 * @brief Digital Input
 * 
 */
class DigitalIn : private NonCopyable<DigitalIn> {
    public:
        /**
         * @brief Construct a new Digital In object
         * 
         * @param pin Pin this DigitalIn is assigned to
         * @param _pinMode The Pinmode, standard is OpenDrain
         */
        DigitalIn(PinName pin, PinMode _pinMode = OpenDrain)
        : _pin(pin) {
            switch (_pinMode) {
                case PullUp:
                    pinMode(pin, INPUT_PULLUP);
                    break;
            
                default:
                    pinMode(pin, INPUT);
                    break;
            }
        }

        /**
         * @brief Read the digital Input
         * 
         * @return uint8_t 1 if HIGH, 0 if LOW
         */
        uint8_t read() {
            if (digitalRead(_pin) == HIGH) {
                return 1;
            } else {
                return 0;
            }
        }

        /**
         * @brief Shorthand for read()
         * 
         * @return uint8_t 1 if HIGH, 0 if LOW
         */
        operator uint8_t() {
            return read();
        }
    
    private:
        PinName _pin;
};

#endif