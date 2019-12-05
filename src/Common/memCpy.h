#ifndef MEMCPY_H
#define MEMCPY_H

#include <stddef.h>

/*
    Copy elements from one location to another.

    @param dest Destination of the copied data
    @param src  Source for the Data to be copied
    @param len  Count of elements to be copied (! NOT BYTES !)
*/
template<typename T>
T* memCpy(T* dest, const T* src, size_t cnt) {
    while (cnt--)
        *dest++ = *src++;
    return dest;
}

#endif // MEMCPY_H