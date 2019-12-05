#ifndef MEMSET_H
#define MEMSET_H

#include <stddef.h>

/*
    Set a bunch of elements to a given value at once

    @param dest Destination of the set data
    @param val  The Value to be Set to all elements
    @param len  Count of elements to be set (! NOT BYTES !)
*/
template<typename T>
T* memSet(T* dest, T val, size_t cnt) {
    while (cnt-- > 0)
        *dest++ = val;
    return dest;
}

#endif