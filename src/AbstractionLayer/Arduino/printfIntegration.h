#ifndef PRINTF_INTEGRATION_H
#define PRINTF_INTEGRATION_H

#include "printf.h"

void _putchar(char character) {
    Serial.write(character);
}

#endif // PRINTF_INTEGRATION_H