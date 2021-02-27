#ifndef VECTOR_H
#define VECTOR_H

#include "NonCopyable.h"
#include "memCpy.h"

#define VECTOR_AUTO_PRERESERVED_SPACE 4
#define VECTOR_AUTO_RESERVE_MULTIPLICATOR 2
#define VECTOR_AUTO_RESERVE_ADDITION 0

namespace std {

template<typename value_type, typename size_type = unsigned int>
class vector : private NonCopyable<vector<value_type, size_type>> {
    public:
        using reference = value_type&;
        using iterator = value_type*;
        using const_iterator = const value_type*;

        vector() = default;

        explicit vector(size_type init_cap) {
            if (init_cap)
                reserve(init_cap);
        }

        ~vector() {
            clear();
        }

        // Element access
        reference at(size_type pos) {
            if (pos >= _currentElementCount)
                return *_begin;

            return _begin[pos];
        }

        reference operator[](size_type pos) {
            return at(pos);
        }

        reference front() {
            return *_begin;
        }

        reference back() {
            if (empty()) {
                return *_begin;
            }

            return _begin[_currentElementCount - 1];
        }

        value_type* data() noexcept {
            if (empty()) {
                return nullptr;
            }

            return _begin;
        }

        // Iterators
        iterator begin() noexcept {
            return _begin;
        }

        iterator end() noexcept {
            return _begin + _currentElementCount;
        }

        // Capacity
        bool empty() const noexcept {
            return _currentElementCount == 0;
        }

        size_type size() const noexcept {
            return _currentElementCount;
        }

        void reserve(size_type new_cap) {
            if (new_cap > _currentSize)
                _changeCapacity(new_cap, _currentElementCount);
        }

        void resize(size_type new_size) {
            if (new_size > _currentElementCount) {
                if (new_size > _currentSize) {
                    reserve(new_size);
                }
            } else {
                for (size_type i = new_size; i < _currentSize; ++i) {
                    _begin[i].~value_type();
                }
                _changeCapacity(new_size, new_size);
            }
        }

        size_type capacity() const noexcept {
            return _currentSize;
        }

        void shrink_to_fit() {
            if (_currentSize > _currentElementCount)
                _changeCapacity(_currentElementCount, _currentElementCount);
        }

        // Modifiers
        void clear() noexcept {
            for (size_type i = 0; i < _currentElementCount; ++i) {
                _begin[i].~value_type();
            }

            _currentElementCount = 0;
            _currentSize = 0;

            if (_begin == _data1) {
                delete[] _rawData1;
            } else if (_begin == _data2) {
                delete _rawData2;
            }

            _data1 = nullptr;
            _data2 = nullptr;
            _begin = nullptr;
        }

        iterator erase(const_iterator pos) {
            if (pos >= end()) return end();

            size_type index = pos - _begin;

            pos->~value_type();

            for (size_type i = index; i < _currentElementCount -
                                          1; ++i) { // -1 because 1 element will be deleted and otherwise it will fail by out of bound by 1
                _begin[i] = _begin[i + 1];
            }

            --_currentElementCount;

            return begin() + index;
        }

        void push_back(const value_type& value) {
            _capacity_increase();
            _begin[_currentElementCount++] = value;
        }

        void push_back(value_type&& val) {
            _capacity_increase();
            _begin[_currentElementCount++] = val;
        }

        void pop_back() {
            _begin[--_currentElementCount].~value_type();
        }

        iterator insert(const_iterator position, const value_type& val) {
            if (position >= end()) return end();

            size_type index = position - _begin;

            _capacity_increase();

            for (size_type i = _currentElementCount; i > index; --i) {
                _begin[i] = _begin[i - 1];
            }
            _begin[index] = val;
            _currentElementCount++;

            return begin() + index;
        }

        iterator insert(const_iterator position, const value_type&& val) {
            if (position >= end()) return end();

            size_type index = position - _begin;

            _capacity_increase();

            for (size_type i = _currentElementCount; i > index; --i) {
                _begin[i] = _begin[i - 1];
            }
            _begin[index] = val;
            _currentElementCount++;

            return begin() + index;
        }

    private:
        size_type _currentSize = 0;
        size_type _currentElementCount = 0;
        value_type* _begin = nullptr;

        value_type* _data1 = nullptr;
        value_type* _data2 = nullptr;

        char* _rawData1 = nullptr;
        char* _rawData2 = nullptr;

        inline void _changeCapacity(size_type new_cap, size_type elementsToCopy) {
            if (_begin == _data1) {
                // Allocate new memory
                _rawData2 = new char[new_cap * sizeof(value_type)];
                _data2 = (value_type*) _rawData2;

                // Copy Data
                memCpy<value_type>(_data2, _data1, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data2;

                // Free up Memory
                delete[] _rawData1;
                _data1 = nullptr;
            } else {
                // Allocate new memory
                _rawData1 = new char[new_cap * sizeof(value_type)];
                _data1 = (value_type*) _rawData1;

                // Copy Data
                memCpy<value_type>(_data1, _data2, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data1;

                // Free up Memory
                delete[] _rawData2;
                _data2 = nullptr;
            }

            _currentSize = new_cap;
        }

        void _capacity_increase(size_type free_space = 1) {
            size_type new_cap = _currentSize;
            if (new_cap == 0) {
                new_cap = VECTOR_AUTO_PRERESERVED_SPACE;
            }

            while (new_cap < _currentElementCount + free_space) {
                new_cap = (new_cap * VECTOR_AUTO_RESERVE_MULTIPLICATOR) + VECTOR_AUTO_RESERVE_ADDITION;
            }

            reserve(new_cap);
        }
};

} // namespace std

#endif // VECTOR_H