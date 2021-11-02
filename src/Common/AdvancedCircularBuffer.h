#ifndef ADVANCED_CIRCULAR_BUFFER_H
#define ADVANCED_CIRCULAR_BUFFER_H

#include "memSet.h"
#include "stdint.h"

template <typename T, size_t BufferSize, typename CounterType = size_t>
class AdvancedCircularBuffer {
   public:
    // ----------------------------------------- Constructors

    /**
     * @brief Construct a new Advanced Circular Buffer
     *
     */
    AdvancedCircularBuffer() : _tail(0), _head(0), _full(false) {}

    /**
     * @brief Construct a new Advanced Circular Buffer object
     *
     * @param forceClearMem explicitely clear the pool if true
     */
    AdvancedCircularBuffer(bool forceClearMem) : AdvancedCircularBuffer() {
        if (forceClearMem) memSet<uint8_t>(_pool, 0, sizeof(_pool));
    }

    // ----------------------------------------- Standard Accessors

    /**
     * @brief Push a new Object to the End of the Container. If the Container is full, the oldest
     * element will be overwritten!
     *
     * @param object
     */
    void push(const T& data) {
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
     * @brief Push a new Object to the Beginning of the Container. If the Container is full, the
     * oldest element will be overwritten! A following pop() would return this element next.
     *
     * @param object
     */
    void pushBack(const T& data) {
        // Check full, if so drop oldest object (not newest)
        if (full()) {
            // Just overwrite tail
            _pool[_tail] = data;
        } else {
            // Calculate new tail
            if (_tail == 0) {
                _tail = BufferSize - 1;
            } else {
                --_tail;
            }

            // Insert into new Tail
            _pool[_tail] = data;

            if (_head == _tail) {
                _full = true;
            }
        }
    }

    /**
     * @brief Pop a Object out of the buffer. The buffer given by the reference will be filled, the
     * original object in the container will be deleted.
     *
     * @param object
     * @return true if the object was written
     * @return false if the container is empty
     */
    bool pop(T& data) {
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
     * @brief Peek the oldest object. The buffer given by the reference will be filled, the original
     * object will stay in the container.
     *
     * @param object
     * @return true if the object got written
     * @return false if the container is empty
     */
    bool peek(T& data) const {
        bool data_updated = false;
        if (!empty()) {
            data = _pool[_tail];
            data_updated = true;
        }
        return data_updated;
    }

    // ----------------------------------------- Advanced Accessors

    /**
     * @brief Get the Head object. The object will be written into the given buffer.
     *
     * @param object
     * @return true if the object was filled
     * @return false if the container is empty
     */
    bool getHead(T& data) const { return getFromHead(0, data); }

    /**
     * @brief Get the Reference to the element at the head. Be careful: no boundary check!
     *
     * @return T&
     */
    T& getHeadReference() { return getReferenceFromHead(0); }

    /**
     * @brief Get the Tail object. The object will be written into the given buffer.
     *
     * @param object
     * @return true if the object was filled
     * @return false if the container is empty
     */
    bool getTail(T& data) const { return getFromTail(0, data); }

    /**
     * @brief Get the Reference to the element at the tail. Be careful: no boundary check!
     *
     * @return T&
     */
    T& getTailReference() { return getReferenceFromTail(0); }

    /**
     * @brief Get the Object with the index-distance from head. The object will be written into the
     * given buffer.
     *
     * @param index
     * @param object
     * @return true if the object was written
     * @return false if the buffer is empty or the index invalid
     */
    bool getFromHead(CounterType index, T& data) const {
        bool data_updated = false;
        if (!empty()) {
            if (size() > index) {
                data = _pool[_getElementIndexFromHead(index)];
                data_updated = true;
            }
        }

        return data_updated;
    }

    /**
     * @brief Get the reference to the element the given distance away from head
     *
     * @param index
     * @return T&
     */
    T& getReferenceFromHead(CounterType index) { return _pool[_getElementIndexFromHead(index)]; }

    /**
     * @brief Get the Object with the index-distance from tail. The object will be written into the
     * given buffer.
     *
     * @param index
     * @param object
     * @return true if the object was written
     * @return false if the buffer is empty or the index invalid
     */
    bool getFromTail(CounterType index, T& data) const {
        bool data_updated = false;
        if (!empty()) {
            if (size() > index) {
                data = _pool[_getElementIndexFromTail(index)];
                data_updated = true;
            }
        }

        return data_updated;
    }

    /**
     * @brief Get the reference to the element the given distance away from tail
     *
     * @param index
     * @return T&
     */
    T& getReferenceFromTail(CounterType index) { return _pool[_getElementIndexFromTail(index)]; }

    // ----------------------------------------- Size -----------------------------------------

    /**
     * @brief Returns true, if the container is full
     *
     * @return true
     * @return false
     */
    bool full() const { return _full; }

    /**
     * @brief Returns true, if the container is empty
     *
     * @return true
     * @return false
     */
    bool empty() const {
        bool is_empty = (_head == _tail) && !_full;
        return is_empty;
    }

    /**
     * @brief Returns the element count currently in the container
     *
     * @return CounterType amount of objects currently in the container
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
     * @brief Returns how many elements can still be stored additionally in the buffer
     *
     * @return CounterType the leftover capacity
     */
    CounterType leftCapacity() const { return BufferSize - size(); }

    // ----------------------------------------- Delete -----------------------------------------

    /**
     * @brief Special function to delete random element in the buffer. The given Index is based on
     * the Head -> index 2 is 2 elements away from the head element.
     *
     * @param index
     * @return true Element got deleted
     * @return false Element did not got deleted (has it even been in the buffer? Invalid Index?)
     */
    bool deleteFromHead(CounterType index) {
        if (index >= size()) return false;

        for (CounterType i = index; i < size() - 1; ++i) {
            _pool[_getElementIndexFromHead(i)] = _pool[_getElementIndexFromHead(i + 1)];
        }

        _tail = _addToCounter(_tail, 1);
        _full = false;

        return true;
    }

    /**
     * @brief Special function to delete random element in the buffer. The given Index is based on
     * the Tail -> index 2 is 2 elements away from the tail element.
     *
     * @param index
     * @return true Element got deleted
     * @return false Element did not got deleted (has it even been in the buffer? Invalid Index?)
     */
    bool deleteFromTail(CounterType index) {
        if (index >= size()) return false;

        for (CounterType i = index; i < size() - 1; ++i) {
            _pool[_getElementIndexFromTail(i)] = _pool[_getElementIndexFromTail(i + 1)];
        }

        _head = _removeFromCounter(_head, 1);
        _full = false;

        return true;
    }

    /**
     * @brief Delete an Amount of messages based on the head
     *
     * @param amount
     * @return true If the element(s) got deleted
     * @return false Invalid amount
     */
    bool deleteAmountFromHead(CounterType amount) {
        if (amount > size() || amount < 0) return false;

        _head = _removeFromCounter(_head, amount);
        _full = false;

        return true;
    }

    /**
     * @brief Delete an Amount of messages based on the tail
     *
     * @param amount
     * @return true If the element(s) got deleted
     * @return false Invalid amount
     */
    bool deleteAmountFromTail(CounterType amount) {
        if (amount > size() || amount < 0) return false;

        _tail = _addToCounter(_tail, amount);
        _full = false;

        return true;
    }

    /**
     * @brief Delete a given Element from the container using its == operator
     *
     * @param element
     * @return true if the element was deleted
     * @return false if the element has not been in the container
     */
    bool deleteElement(T& element) {
        CounterType currentSize = size();

        for (CounterType i = 0; i < currentSize; ++i) {
            if (_pool[_getElementIndexFromTail(i)] == element) {
                if (i < (currentSize - 1) / 2) {
                    return deleteFromHead(--currentSize -
                                          i);   // currentSize will not be used anymore, so using
                                                // this instead of currentsize-1
                } else {
                    return deleteFromTail(i);
                }
            }
        }

        return false;
    }

    /**
     * @brief Reset the container
     *
     */
    void reset() {
        _head = 0;
        _tail = 0;
        _full = false;
    }

    /**
     * @brief Drop the oldest object (the tail-object)
     *
     * @return true if the oldest Message got dropped
     * @return false if the container is empty
     */
    bool dropLast() {
        bool data_dropped = false;
        if (!empty()) {
            if (++_tail == BufferSize) {
                _tail = 0;
            }
            _full = false;
            data_dropped = true;
        }
        return data_dropped;
    }

   private:
    T _pool[BufferSize];
    CounterType _tail, _head;
    bool _full;

    // ----------------------------------------- Addition / Subtraction

    /**
     * @brief Add a given Amount to a given counter
     *
     * @param counter
     * @param amount
     * @return CounterType The new counter value
     */
    CounterType _addToCounter(CounterType counter, CounterType amount) const {
        CounterType counterSubtract;
        amount %= BufferSize;

        counterSubtract = BufferSize - amount;
        counterSubtract %= BufferSize;

        return _removeFromCounter(counter, counterSubtract);
    }

    /**
     * @brief Remove a given Amount from a given counter
     *
     * @param counter
     * @param amount
     * @return CounterType The new counter value
     */
    CounterType _removeFromCounter(CounterType counter, CounterType amount) const {
        CounterType newCounterVal;
        amount %= BufferSize;

        if (counter < amount) {
            newCounterVal = amount - counter;
            newCounterVal = BufferSize - newCounterVal;
        } else {
            newCounterVal = counter - amount;
        }

        return newCounterVal;
    }

    // ----------------------------------------- Index Calculators

    /**
     * @brief Get the elementIndex (to be used for accessing elements in the _pool) by giving an
     * index, representing the index based on the head (index = 0 == last element index)
     *
     * @param index
     * @return CounterType
     */
    CounterType _getElementIndexFromHead(CounterType index) const {
        index %= BufferSize;
        ++index;   // ++ because the head is pointing to the element behind the last element

        return _removeFromCounter(_head, index);
    }

    /**
     * @brief Get the elementIndex (to be used for accessing elements in the _pool) by giving an
     * index, representing the index based on the tail (index = 0 == first element index)
     *
     * @param index
     * @return CounterType
     */
    CounterType _getElementIndexFromTail(CounterType index) const {
        return _addToCounter(_tail, index);
    }
};

#endif
