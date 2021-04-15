#ifndef MEMSET_H
#define MEMSET_H

#include <stddef.h>

/**
 * @brief Set a bunch of elements to a given value at once
 * 
 * @tparam T 
 * @param dest Destination of the set data
 * @param val The Value to be Set to all elements
 * @param count Count of elements to be set (! NOT BYTES !)
 * @return T* 
 */
template<typename T>
T* memSet(T* dest, T val, size_t count) {
    while (count-- > 0)
        *dest++ = val;
    return dest;
}

#endif