#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_AUTO_PRERESERVED_SPACE 4
#define VECTOR_AUTO_RESERVE_MULTIPLICATOR 2
#define VECTOR_AUTO_RESERVE_ADDITION 0

namespace std {

template <class T>
using iterator = T*;

template<class T, typename counter_type_t = unsigned int>
class vector : private NonCopyable<vector<T, counter_type_t>> {
    public:
        // Destructor
        ~vector() {
            clear();
        }

        // Element access
        T& at(counter_type_t pos) {
            if (pos >= _currentElementCount)
                return *_begin;
            
            return _begin[pos];
        }

        T& operator[](counter_type_t pos) {
            return at(pos);
        }

        T& front() {
            return *_begin;
        }

        T& back() {
            if (empty()) {
                return *_begin;
            }

            return begin[_currentElementCount - 1];
        }

        T* data() {
            if (empty()) {
                return nullptr;
            }
            
            return _begin;
        }

        // Iterators
        iterator<T> begin() {
            return _begin;
        }

        iterator<T> end() {
            return _begin + _currentElementCount;
        }

        // Capacity
        bool empty() {
            return _currentElementCount == 0;
        }

        counter_type_t size() {
            return _currentElementCount;
        }

        void reserve(counter_type_t new_cap) {
            if (new_cap > _currentSize)
                _changeCapacity(new_cap, _currentElementCount);
        }
        
        counter_type_t capacity() {
            return _currentSize;
        }

        void shrink_to_fit() {
            if (_currentSize > _currentElementCount)
                _changeCapacity(_currentElementCount, _currentElementCount);
        }

        // Modifiers
        void clear() {
            _currentElementCount = 0;
            _currentSize = 0;

            if (_begin == _data1) {
                delete _rawData1;
            } else if (_begin == _data2) {
                delete _rawData2;
            }

            _data1 = nullptr;
            _data2 = nullptr;
            _begin = nullptr;
        }

        iterator<T> erase(iterator<T> pos) {
            if (pos >= end()) return pos;

            counter_type_t index = pos - _begin;

            pos->~T();

            for (counter_type_t i = index; i < _currentElementCount - 1; ++i) { // -1 because 1 element will be deleted and otherwise it will fail by out of bound by 1
                _begin[i] = _begin[i+1];
            }

            --_currentElementCount;

            return pos;
        }

        void push_back(T& value) {
            // Get Memory for new element
            if (_currentSize == 0)
                reserve(VECTOR_AUTO_PRERESERVED_SPACE);
            else if (_currentSize == _currentElementCount)
                reserve((_currentSize * VECTOR_AUTO_RESERVE_MULTIPLICATOR) + VECTOR_AUTO_RESERVE_ADDITION);

            // Copy element
            _begin[_currentElementCount++] = value;
        }

        void pop_back() {
            --_currentElementCount;
        }
    
    private:
        counter_type_t _currentSize = 0;
        counter_type_t _currentElementCount = 0;
        T* _begin = nullptr;

        T* _data1 = nullptr;
        T* _data2 = nullptr;

        char* _rawData1 = nullptr;
        char* _rawData2 = nullptr;

        inline void _changeCapacity(counter_type_t new_cap, counter_type_t elementsToCopy) {
            if (_begin == _data1) {
                // Allocate new memory
                _rawData2 = new char[new_cap * sizeof(T)];
                _data2 = (T*)_rawData2;

                // Copy Data
                memCpy<T>(_data2, _data1, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data2;

                // Free up Memory
                delete _rawData1;
                _data1 = nullptr;
            } else {
                // Allocate new memory
                _rawData1 = new char[new_cap * sizeof(T)];
                _data1 = (T*)_rawData1;

                // Copy Data
                memCpy<T>(_data1, _data2, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data1;

                // Free up Memory
                delete _rawData2;
                _data2 = nullptr;
            }

            _currentSize = new_cap;
        }
};

}; // namespace std

#endif // VECTOR_H