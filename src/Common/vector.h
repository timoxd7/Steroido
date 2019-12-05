#ifndef VECTOR_H
#define VECTOR_H

#define VECTOR_AUTO_PRERESERVED_SPACE 4
#define VECTOR_AUTO_RESERVE_MULTIPLICATOR 2
#define VECTOR_AUTO_RESERVE_ADDITION 0

template<class T, typename counter_type_t>
class iterator {
    public:
        iterator(T& element, counter_type_t pos)
        : _ptr(&element), _pos(pos) {}

        iterator(T* ptr, counter_type_t pos)
        : _ptr(ptr), _pos(pos) {}

        T& operator*() {
            return *_ptr;
        }

        bool operator==(iterator<T, counter_type_t> that) {
            return _ptr == that.getPtr() && _pos == that.getPos();
        }

        bool operator!=(iterator<T, counter_type_t> that) {
            return _ptr != that.getPtr() || _pos != that.getPos();
        }

        iterator<T, counter_type_t> operator++(int) { // -> something++
            iterator<T, counter_type_t> oldThis = *this;

            ++_ptr;
            ++_pos;

            return oldThis;
        }

        iterator<T, counter_type_t>& operator++() { // -> ++something
            ++_ptr;
            ++_pos;

            return *this;
        }

        iterator<T, counter_type_t> operator--(int) { // -> something--
            iterator<T, counter_type_t> oldThis = *this;

            --_ptr;
            --_pos;

            return oldThis;
        }

        iterator<T, counter_type_t>& operator--() { // -> --something
            --_ptr;
            --_pos;

            return *this;
        }

        counter_type_t getPos() {
            return _pos;
        }

        T* getPtr() {
            return _ptr;
        }

    private:
        T* _ptr;
        counter_type_t _pos;
};

template<class T, typename counter_type_t = unsigned int>
class vector {
    public:
        // Destructor
        ~vector() {
            delete _begin;
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
            if (empty()) {
                return *_begin;
            }

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
        iterator<T, counter_type_t> begin() {
            return iterator<T, counter_type_t>(_begin, 0);
        }

        iterator<T, counter_type_t> end() {
            return iterator<T, counter_type_t>(_begin + _currentElementCount, _currentElementCount);
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
                delete _data1;
            } else if (_begin == _data2) {
                delete _data2;
            }

            _data1 = nullptr;
            _data2 = nullptr;
            _begin = nullptr;
        }

        iterator<T, counter_type_t> erase(iterator<T, counter_type_t> pos) {
            if (pos.getPos() >= _currentElementCount) return pos;
            if (pos.getPtr() != _begin + pos.getPos()) return pos;

            for (counter_type_t i = pos.getPos(); i < _currentElementCount - 1; ++i) { // -1 because 1 element will be deleted and otherwise it will fail by out of bound by 1
                _begin[i] = _begin[i+1];
            }

            --_currentElementCount;

            return iterator<T, counter_type_t>(_begin + pos.getPos(), pos.getPos());
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

        inline void _changeCapacity(counter_type_t new_cap, counter_type_t elementsToCopy) {
            if (_begin == _data1) {
                // Allocate new memory
                _data2 = new T[new_cap];

                // Copy Data
                memCpy<T>(_data2, _data1, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data2;

                // Free up Memory
                delete _data1;
            } else {
                // Allocate new memory
                _data1 = new T[new_cap];

                // Copy Data
                memCpy<T>(_data1, _data2, elementsToCopy);

                // Change begin pointer to new data location
                _begin = _data1;

                // Free up Memory
                delete _data2;
            }

            _currentSize = new_cap;
        }
};

#endif // VECTOR_H