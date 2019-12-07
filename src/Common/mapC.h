#ifndef MAPC_H
#define MAPC_H

template<typename T>
T mapC(T x, T in_min, T in_max, T out_min, T out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // MAPC_H