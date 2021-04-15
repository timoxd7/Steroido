#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

/**
 * @brief Buffer to save things in static storage in a circle
 * 
 * @tparam T The Type of the saved object/datatype
 * @tparam BufferSize The Size of the buffer (element count)
 * @tparam CounterType The Type the buffer will count the elements
 * @note CounterType must be unsigned and consistent with BufferSize
 */
template<typename T, uint16_t BufferSize, typename CounterType = uint16_t>
class CircularBuffer {
    public:
        /**
         * @brief Construct a new Circular Buffer
         * 
         */
        CircularBuffer() : _head(0), _tail(0), _full(false) {}
        ~CircularBuffer() {}

        /**
         * @brief Push the data to the buffer. This overwrites the buffer if it's full
         * 
         * @param data Data to be pushed to the buffer
         */
        void push(const T &data) {
            if (full()) {
                _tail++;
                if (_tail == BufferSize) {
                    _tail = 0;
                }
            }
            _pool[_head++] = data;
            if (_head == BufferSize) {
                _head = 0;
            }
            if (_head == _tail) {
                _full = true;
            }
        }

        /**
         * @brief Pop data from the buffer
         * 
         * @param data Data to be popped from the buffer
         * @return true if the buffer is not empty, false otherwise
         */
        bool pop(T &data) {
            bool data_popped = false;
            if (!empty()) {
                data = _pool[_tail++];
                if (_tail == BufferSize) {
                    _tail = 0;
                }
                _full = false;
                data_popped = true;
            }
            return data_popped;
        }

        /**
         * @brief Check if the buffer is empty
         * 
         * @return true if the buffer is empty
         * @return false if one or more elements are in the buffer
         */
        bool empty() const {
            bool is_empty = (_head == _tail) && !_full;
            return is_empty;
        }

        /**
         * @brief Check if the buffer is full
         * 
         * @return true if the buffer is full
         * @return false if one or more places are empty
         */
        bool full() const {
            bool full = _full;
            return full;
        }
        
        /**
         * @brief Reset the Buffer, marks all Elements as free
         * 
         */
        void reset() {
            _head = 0;
            _tail = 0;
            _full = false;
        }

        /**
         * @brief Returns the number of elements currently stored in the buffer
         * 
         * @return CounterType number of elements currently stored in the buffer
         */
        CounterType size() const {
            CounterType elements;
            if (!_full) {
                if (_head < _tail) {
                    elements = BufferSize + _head - _tail;
                } else {
                    elements = _head - _tail;
                }
            } else {
                elements = BufferSize;
            }
            return elements;
        }

        /**
         * @brief Peek the oldest Element without popping
         * 
         * @param data Oldest Element in Buffer
         * @return true If the buffer is not empty
         * @return false If the buffer is empty and nothing got peeked
         */
        bool peek(T &data) const {
            bool data_updated = false;
            if (!empty()) {
                data = _pool[_tail];
                data_updated = true;
            }
            return data_updated;
        }

    private:
        T _pool[BufferSize];
        CounterType _head;
        CounterType _tail;
        bool _full;
};

#endif
