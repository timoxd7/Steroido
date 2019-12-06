#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

/*
    Templated Circular buffer class
    @note CounterType must be unsigned and consistent with BufferSize
*/
template<typename T, uint16_t BufferSize, typename CounterType = uint16_t>
class CircularBuffer : private NonCopyable<CircularBuffer<T, BufferSize, CounterType>> {
    public:
        CircularBuffer() : _head(0), _tail(0), _full(false) {}
        ~CircularBuffer() {}

        /*
            Push the transaction to the buffer. This overwrites the buffer if it's full
            @param data Data to be pushed to the buffer
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

        /*
            Pop the transaction from the buffer

            @param data Data to be popped from the buffer
            @return True if the buffer is not empty and data contains a transaction, false otherwise
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

        /*
            Check if the buffer is empty

            @return True if the buffer is empty, false if not
        */
        bool empty() const {
            bool is_empty = (_head == _tail) && !_full;
            return is_empty;
        }

        /*
            Check if the buffer is full

            @return True if the buffer is full, false if not
        */
        bool full() const {
            bool full = _full;
            return full;
        }

        /*
            Reset the buffer
        */
        void reset() {
            _head = 0;
            _tail = 0;
            _full = false;
        }

        // Get the number of elements currently stored in the circular_buffer
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

        /*
            Peek into circular buffer without popping

            @param data Data to be peeked from the buffer
            @return True if the buffer is not empty and data contains a transaction, false otherwise
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
