#ifndef DIGITAL_IN_H
#define DIGITAL_IN_H

class DigitalIn : private NonCopyable<DigitalIn> {
    public:
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

        uint8_t read() {
            if (digitalRead(_pin) == HIGH) {
                return 1;
            } else {
                return 0;
            }
        }

        operator uint8_t() {
            return read();
        }
    
    private:
        PinName _pin;
};

#endif