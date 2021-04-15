#ifndef PINMODE_H
#define PINMODE_H

/**
 * @brief The Mode a Input Pin should be in. Most Arduinos only support PullUp or OpenDrain!
 * 
 */
enum PinMode : uint8_t {
    PullUp = 0,
    PullDown,
    PullNone,
    OpenDrain
};

#endif // PINMODE_H