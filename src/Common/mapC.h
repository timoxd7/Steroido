#ifndef MAPC_H
#define MAPC_H

/**
 * @brief Map a value from a scale to a new one
 * 
 * @tparam T Datatype of the given values
 * @param x value to map
 * @param in_min 
 * @param in_max 
 * @param out_min 
 * @param out_max 
 * @return T mapped value
 */
template<typename T>
T mapC(T x, T in_min, T in_max, T out_min, T out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // MAPC_H