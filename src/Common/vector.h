#ifndef VECTOR_H
#define VECTOR_H

#include <new>
#include "NonCopyable.h"
#include "memCpy.h"

#define VECTOR_AUTO_PRERESERVED_SPACE 4
#define VECTOR_AUTO_RESERVE_MULTIPLICATOR 2
#define VECTOR_AUTO_RESERVE_ADDITION 0

namespace std {

template<class T, typename counter_type = unsigned int>
class vector : private NonCopyable<vector<T, counter_type>> {
    public:
        typedef T& reference;
        typedef T* pointer;
        typedef pointer iterator;
        typedef const T* const_iterator;

        vector() = default;

        explicit vector(counter_type init_cap) {
            if (init_cap)
                reserve(init_cap);
        }

        ~vector() {
            clear();
        }

        // Element access
        reference at(counter_type pos) {
            if (pos >= _currentElementCount)
                return *_begin;

            return _begin[pos];
        }

        reference operator[](counter_type pos) {
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

        T* data() noexcept {
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

        counter_type size() const noexcept {
            return _currentElementCount;
        }

        /**
         * @brief Attempt to preallocate enough memory for specified number of elements.
         * 
         * @param new_cap 
         */
        void reserve(counter_type new_cap) {
            if (new_cap > _currentSize)
                _changeCapacity(new_cap, _currentElementCount);
        }

        /**
         * @brief Resize to the new whished size. After this, [new_size] elements are inside of the vector
         * 
         * @param new_size The new amount of elements for this vector
         */
        void resize(counter_type new_size) {
            if (new_size > _currentElementCount) {
                if (new_size > _currentSize) {
                    reserve(new_size);
                }

                for (counter_type i = _currentElementCount; i < new_size; ++i) {
                    new(_begin + i) T();
                }

                _currentElementCount = new_size;
            } else {
                for (counter_type i = new_size; i < _currentSize; ++i) {
                    _begin[i].~T();
                }
                _currentElementCount = new_size;
                _changeCapacity(new_size, new_size);
            }
        }

        counter_type capacity() const noexcept {
            return _currentSize;
        }

        void shrink_to_fit() {
            if (_currentSize > _currentElementCount)
                _changeCapacity(_currentElementCount, _currentElementCount);
        }

        // Modifiers
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

        iterator erase(iterator pos) {
            if (pos >= end()) return end();

            counter_type index = pos - _begin;

            pos->~T();

            --_currentElementCount; // -1 because 1 element will be deleted and otherwise it will fail by out of bound by 1
            for (counter_type i = index; i < _currentElementCount; ++i) {
                _begin[i] = _begin[i + 1];
            }


            return pos;
        }

        void push_back(const T& value) {
            _checkOneFree();
            _begin[_currentElementCount++] = value;
        }

        void push_back(T&& val) {
            _checkOneFree();
            _begin[_currentElementCount++] = val;
        }

        void pop_back() {
            _begin[--_currentElementCount].~T();
        }

        iterator insert(const_iterator position, const T& val) {
            return _insert<T&>(position, val);
        }

        iterator insert(const_iterator position, const T&& val) {
            return _insert<T&&>(position, val);
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
         * @brief Changes Capacity for the container to the new wished size. Does NOT call any construct- or destructors. Does also NOT change _currentElementCount!
         * 
         * @param new_cap The new Capacity
         * @param elementsToCopy The amount of elements to copy to the newly allocated memory
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
         * @brief Makes sure that at least one free slot is avaliable
         * 
         * @param free_space 
         */
        void _checkOneFree() {
            if (_currentSize == 0)
                reserve(VECTOR_AUTO_PRERESERVED_SPACE);
            else if (_currentSize == _currentElementCount)
                reserve((_currentSize * VECTOR_AUTO_RESERVE_MULTIPLICATOR) + VECTOR_AUTO_RESERVE_ADDITION);
        }

        /**
         * @brief Inserts the given Element
         * 
         * @tparam T 
         * @param position 
         * @param val 
         */
        template<typename R>
        inline iterator _insert(const_iterator position, const R val) {
            if (position >= end()) return end();

            counter_type index = position - _begin;

            _checkOneFree();

            for (counter_type i = _currentElementCount; i > index; --i) {
                _begin[i] = _begin[i - 1];
            }
            _begin[index] = val;
            _currentElementCount++;

            return begin() + index;
        }
};

} // namespace std

#endif // VECTOR_H