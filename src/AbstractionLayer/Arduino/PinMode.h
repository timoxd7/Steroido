#ifndef PINMODE_H
#define PINMODE_H

enum PinMode : uint8_t {
    PullUp = 0,
    PullDown,
    PullNone,
    OpenDrain
};

#endif // PINMODE_H