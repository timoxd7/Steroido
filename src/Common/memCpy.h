#ifndef MEMCPY_H
#define MEMCPY_H

#include <stddef.h>

/**
 * @brief Copy elements from one location to another.
 * 
 * @tparam T 
 * @param dest Destination of the copied data
 * @param src Source for the Data to be copied
 * @param count Count of elements to be copied (! NOT BYTES !)
 * @return T* Pointer to the Destination
 */
template<typename T>
T* memCpy(T* dest, const T* src, size_t count) {
    while (count--)
        *dest++ = *src++;
    return dest;
}

#endif // MEMCPY_H