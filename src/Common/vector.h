#ifndef VECTOR_H
#define VECTOR_H

#include "NonCopyable.h"
#include "memCpy.h"

#define VECTOR_AUTO_PRERESERVED_SPACE 4
#define VECTOR_AUTO_RESERVE_MULTIPLICATOR 2
#define VECTOR_AUTO_RESERVE_ADDITION 0

namespace std {

template<class T, typename counter_type = unsigned int>
class vector : private NonCopyable<vector<T, counter_type>> {
    public:
        using reference = T&;
        using pointer = T*;
        using iterator = pointer;

        /**
         * Creates a vector with zero capacity.
         */
        vector() = default;

        /**
         * Creates a vector with the given capacity.
         * @param init_cap Number of elements, that should fit inside.
         */
        explicit vector(counter_type init_cap) {
            if (init_cap) {
                reserve(init_cap);
            }
        }

        /**
         * Destructs this vector and releases memory.
         */
        ~vector() {
            clear();
        }

        /**
         * Element access.
         * @param pos Position of the element.
         * @return Element at that position.
         */
        reference at(counter_type pos) {
            if (pos >= _currentElementCount) {
                return *_begin;
            }
            return _begin[pos];
        }

        /**
         * Element access.
         * @param pos Position of the element.
         * @return Element at that position.
         */
        reference operator[](counter_type pos) {
            return at(pos);
        }

        /**
         * Element at the front.
         * @return First element.
         */
        reference front() {
            return *_begin;
        }

        /**
         * Element at the end.
         * @return Last element.
         */
        reference back() {
            if (empty()) {
                return *_begin;
            }
            return _begin[_currentElementCount - 1];
        }

        /**
         * Raw data array the elements are stored in.
         * @return Pointer to that array.
         */
        T* data() noexcept {
            if (empty()) {
                return nullptr;
            }
            return _begin;
        }

        /**
         * Iterator on the first element.
         * @return Iterator.
         */
        iterator begin() noexcept {
            return _begin;
        }

        /**
         * Iterator on the after-last element.
         * @return Iterator.
         */
        iterator end() noexcept {
            return _begin + _currentElementCount;
        }

        /**
         * If there are no elements inside.
         * @return If empty.
         */
        bool empty() const noexcept {
            return _currentElementCount == 0;
        }

        /**
         * Current number of elements inside.
         * @return Element count.
         */
        counter_type size() const noexcept {
            return _currentElementCount;
        }

        /**
         * Allocates memory for the specified number of elements.
         * @param new_cap Total number of elements.
         */
        void reserve(counter_type new_cap) {
            if (new_cap > _currentSize) {
                _changeCapacity(new_cap, _currentElementCount);
            }
        }

        /**
         * How many elements can fit inside without reallocation.
         * @return Current capacity.
         */
        counter_type capacity() const noexcept {
            return _currentSize;
        }

        /**
         * Reallocates memory so only the currently contained elements fit inside.
         */
        void shrink_to_fit() {
            if (_currentSize > _currentElementCount) {
                _changeCapacity(_currentElementCount, _currentElementCount);
            }
        }

        /**
         * Removes all elements.
         */
        void clear() noexcept {
            for (counter_type i = 0; i < _currentElementCount; ++i) {
                _begin[i].~T();
            }

            _currentElementCount = 0;
            _currentSize = 0;

            if (_begin == _data1) {
                delete[] _rawData1;
            } else if (_begin == _data2) {
                delete[] _rawData2;
            }

            _data1 = nullptr;
            _data2 = nullptr;
            _begin = nullptr;
        }

        /**
         * Removes one element.
         * @param pos Position of the element.
         * @return Position the element had been at.
         */
        iterator erase(iterator pos) {
            if (pos >= end()) {
                return end();
            }

            counter_type index = pos - _begin;

            pos->~T();

            --_currentElementCount; // -1 because 1 element will be deleted and otherwise it will fail by out of bound by 1
            for (counter_type i = index; i < _currentElementCount; ++i) {
                _begin[i] = _begin[i + 1];
            }


            return pos;
        }

        /**
         * Inserts an element at the end.
         * @param value Element to insert.
         */
        void push_back(const T& value) {
            _capacity_free();
            _begin[_currentElementCount++] = value;
        }

        /**
         * Inserts an element at the end.
         * @param value Element to insert.
         */
        void push_back(const T&& value) {
            _capacity_free();
            _begin[_currentElementCount++] = value;
        }

        /**
         * Removes the element currently at the end.
         */
        void pop_back() {
            _begin[--_currentElementCount].~T();
        }

        /**
         * Inserts an element at the specified position.
         * Elements at or behind that position will be shifted back.
         * @param pos Position the element will be placed.
         * @param value Element itself.
         * @return Position of the inserted element.
         */
        iterator insert(iterator pos, const T& value) {
            return _insert<T&>(pos, value);
        }

        /**
         * Inserts an element at the specified position.
         * Elements at or behind that position will be shifted back.
         * @param pos Position the element will be placed.
         * @param value Element itself.
         * @return Position of the inserted element.
         */
        iterator insert(iterator pos, const T&& value) {
            return _insert<T&&>(pos, value);
        }

    private:
        counter_type _currentSize = 0;
        counter_type _currentElementCount = 0;
        pointer _begin = nullptr;

        pointer _data1 = nullptr;
        pointer _data2 = nullptr;

        char* _rawData1 = nullptr;
        char* _rawData2 = nullptr;

        /**
         * Changes the capacity to the specified size and copies the specified number of elements into the newly allocated memory.
         * Does not deconstruct elements that are not copied and does not alter the current element count.
         * @param new_cap New capacity.
         * @param elementsToCopy Number of elements to copy.
         */
        inline void _changeCapacity(counter_type new_cap, counter_type elementsToCopy) {
            if (_begin == _data1) {
                // Allocate new memory
                _rawData2 = new char[new_cap * sizeof(T)];
                _data2 = (pointer) _rawData2;

                // Copy Data
                memCpy<T>(_data2, _data1, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data2;

                // Free up Memory
                delete[] _rawData1;
                _data1 = nullptr;
            } else {
                // Allocate new memory
                _rawData1 = new char[new_cap * sizeof(T)];
                _data1 = (pointer) _rawData1;

                // Copy Data
                memCpy<T>(_data1, _data2, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data1;

                // Free up Memory
                delete[] _rawData2;
                _data2 = nullptr;
            }

            _currentSize = new_cap;
        }

        /**
         * Makes sure at least one more element can fit inside.
         */
        void _capacity_free() {
            if (_currentSize == 0) {
                reserve(VECTOR_AUTO_PRERESERVED_SPACE);
            } else if (_currentSize == _currentElementCount) {
                reserve((_currentSize * VECTOR_AUTO_RESERVE_MULTIPLICATOR) + VECTOR_AUTO_RESERVE_ADDITION);
            }
        }

        /**
        * Inserts an element at the specified position.
        * Elements at or behind that position will be shifted back.
        * @tparam R value, reference or r-value reference of T.
        * @param pos Position the element will be placed.
        * @param value Element itself.
        * @return Position of the inserted element.
        */
        template<typename R>
        inline iterator _insert(iterator pos, const R value) {
            if (pos >= end()) {
                return end();
            }
            counter_type index = pos - _begin;

            _capacity_free();

            for (counter_type i = _currentElementCount; i > index; --i) {
                _begin[i] = _begin[i - 1];
            }
            _begin[index] = value;
            _currentElementCount++;

            return pos;
        }

};

} // namespace std

#endif // VECTOR_H