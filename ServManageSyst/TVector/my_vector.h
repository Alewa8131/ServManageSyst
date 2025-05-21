// Copyright 2025 Alewa8131
#pragma once
#include <iostream>
#include <initializer_list>
#include <random>
#include <cstddef>

extern std::mt19937 engine;

enum State { empty, busy, deleted };

template <class T>
class TVector {
    T* _data = nullptr;
    State* _states = nullptr;
    size_t _capacity = 0;
    size_t _size = 0;
    size_t _deleted = 0;

 public:
     TVector();
     explicit TVector(size_t size);
     TVector(std::initializer_list<T> init);
     template <size_t N>
     explicit TVector(const T(&arr)[N]);
     TVector(const T* arr, size_t size);
     TVector(const TVector& other);

     ~TVector();

     inline bool is_empty() const noexcept;
     inline size_t size() const noexcept;
     inline size_t capacity() const noexcept;
     inline T* data() noexcept;
     inline T* begin() noexcept;
     inline T* end() noexcept;
     inline T& front() noexcept;

     T& back() noexcept;
     T& at(size_t pos);
     const T& at(size_t pos) const;

     void push_front(const T& value);
     void push_back(const T& value);
     void insert(size_t pos, const T& value);

     void pop_front();
     void pop_back();
     void erase(size_t pos);

     void emplace(size_t pos, const T& value);
     void clear();
     void assign(size_t count, const T& value);

     void shrink_to_fit();

     void resize(size_t new_size);

     bool operator==(const TVector& other) const;
     bool operator!=(const TVector& other) const;
     TVector& operator=(const TVector& other);
     T& operator[](size_t pos) noexcept;
     const T& operator[](size_t pos) const noexcept;

     template <class T>
     friend void shuffle(TVector<T>& vec);
     template <class T>
     friend int find_first(TVector<T>& vec, const T& value);
     template <class T>
     friend int find_last(TVector<T>& vec, const T& value);
     template <class T>
     friend TVector<size_t> find_all(const TVector<T>& vec, const T& value);

 private:
     inline bool is_full() const noexcept;
     void ensure_capacity(size_t start_index = 0);
     void reserve(size_t new_cap, size_t start_index = 0);
     void clear_tail(T* data, State* states, size_t from, size_t to);
     void free_memory();

     template <typename InputIt, typename OutputIt>
     void my_copy(InputIt first, InputIt last, OutputIt dest);
};

template <class T>
TVector<T>::TVector() = default;

template <class T>
TVector<T>::TVector(size_t size)
    : _data(new T[size]), _capacity(size), _size(0), _states(new State[size]) {
    std::fill_n(_states, size, empty);
}

template <class T>
TVector<T>::TVector(std::initializer_list<T> init) :
    _data(new T[init.size()]),
    _capacity(init.size()),
    _size(init.size()),
    _states(new State[init.size()]) {
    my_copy(init.begin(), init.end(), _data);
    std::fill_n(_states, _size, busy);
    std::fill_n(&_states[_size], _capacity - _size, empty);
}

template <class T>
template <size_t N>
TVector<T>::TVector(const T(&arr)[N]) : TVector(arr, N) {}

template <class T>
TVector<T>::TVector(const T* arr, size_t size) : TVector(size) {
    for (size_t i = 0; i < size; ++i) {
        _data[i] = arr[i];
        _states[i] = busy;
        ++_size;
    }
}

template <class T>
TVector<T>::TVector(const TVector& other) :
    _data(new T[other._capacity]), _states(new State[other._capacity]),
    _capacity(other._capacity), _size(other._size), _deleted(other._deleted) {
    my_copy(other._data, other._data + other._capacity, _data);
    my_copy(other._states, other._states + other._capacity, _states);
}

template <class T>
TVector<T>::~TVector() {
    if (_data != nullptr) {
        delete[] _data;
        delete[] _states;
        _data = nullptr;
        _states = nullptr;
    }
}

template <class T>
inline bool TVector<T>::is_empty() const noexcept { return _size == 0; }
template <class T>
inline size_t TVector<T>::size() const noexcept { return _size; }
template <class T>
inline size_t TVector<T>::capacity() const noexcept { return _capacity; }
template <class T>
inline T* TVector<T>::data() noexcept { return _data; }
template <class T>
inline T* TVector<T>::begin() noexcept { return _data; }
template <class T>
inline T* TVector<T>::end() noexcept { return _data + _capacity; }
template <class T>
inline T& TVector<T>::front() noexcept { return at(0); }
template <class T>
T& TVector<T>::back() noexcept {
    for (size_t i = _capacity; i-- > 0;)
        if (_states[i] == busy)
            return _data[i];
}
template <class T>
T& TVector<T>::at(size_t pos) {
    if (pos >= _size) throw std::out_of_range("TVector::at");

    size_t count = 0;
    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            if (count == pos) return _data[i];
            ++count;
        }
    }
    throw std::out_of_range("TVector::at - logical error");
}
template <class T>
const T& TVector<T>::at(size_t pos) const {
    if (pos >= _size) throw std::out_of_range("TVector::at");

    size_t count = 0;
    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            if (count == pos) return _data[i];
            ++count;
        }
    }
    throw std::out_of_range("TVector::at - logical error");
}

template <class T>
void TVector<T>::push_front(const T& value) {
    ensure_capacity(1);

    // If the first element is not busy, we insert it without a shift
    if (_states[0] != busy) {
        _data[0] = value;
        if (_states[0] == deleted) {
            --_deleted;
        }
        _states[0] = busy;
        ++_size;
        return;
    }

    // We are looking for a position where you can move
    // (the last empty or remote)
    size_t target = _capacity;
    for (size_t i = _capacity; i-- > 0;) {
        if (_states[i] != busy) {
            target = i;
            break;
        }
    }

    // We move the elements back to the found position
    for (size_t i = target; i > 0; --i) {
        _data[i] = _data[i - 1];
        _states[i] = _states[i - 1];
    }

    _data[0] = value;
    _states[0] = busy;
    ++_size;
}
template <class T>
void TVector<T>::push_back(const T& value) {
    ensure_capacity();
    _data[_size] = value;
    _states[_size] = busy;
    ++_size;
}
template <class T>
void TVector<T>::insert(size_t pos, const T& value) {
    ensure_capacity();

    // We are looking for a physical index of the logical position "pos"
    size_t busy_count = 0;
    size_t physical_index = 0;
    for (; physical_index < _capacity; ++physical_index) {
        if (_states[physical_index] == busy) {
            if (busy_count == pos) break;
            ++busy_count;
        }
    }

    if (busy_count < pos) {
        push_back(value);
        return;
    }

    size_t left = physical_index;
    while (left > 0) {
        if (_states[left - 1] != busy) break;
        --left;
    }

    bool can_shift_left = (_states[left - 1] != busy
        && left - 1 < physical_index);
    size_t right = _capacity;
    for (size_t i = _capacity; i-- > physical_index;) {
        if (_states[i] != busy) {
            right = i;
            break;
        }
    }

    bool can_shift_right = (right < _capacity);
    if (can_shift_left) {
        for (size_t i = left - 1; i < physical_index - 1; ++i) {
            _data[i] = _data[i + 1];
            _states[i] = _states[i + 1];
        }
        _data[physical_index - 1] = value;
        _states[physical_index - 1] = busy;
        ++_size;
        if (_states[physical_index - 1] == deleted) --_deleted;
        return;
    }

    if (can_shift_right) {
        for (size_t i = right; i > physical_index; --i) {
            _data[i] = _data[i - 1];
            _states[i] = _states[i - 1];
        }
        _data[physical_index] = value;
        _states[physical_index] = busy;
        ++_size;
        if (_states[physical_index] == deleted) --_deleted;
        return;
    }

    push_back(value);
}

template <class T>
void TVector<T>::pop_front() {
    if (_size == 0) return;

    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            _states[i] = deleted;
            ++_deleted;
            --_size;
            return;
        }
    }
}
template <class T>
void TVector<T>::pop_back() {
    if (_size == 0) return;

    for (size_t i = _capacity; i-- > 0;) {
        if (_states[i] == busy) {
            _states[i] = empty;
            --_size;
            return;
        }
    }
}
template <class T>
void TVector<T>::erase(size_t pos) {
    if (_size == 0 || pos >= _size || pos < 0) return;

    size_t busy_count = 0;
    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            if (busy_count == pos) {
                _states[i] = deleted;
                --_size;
                ++_deleted;
                return;
            }
            ++busy_count;
        }
    }
}

template <class T>
void TVector<T>::emplace(size_t pos, const T& value) {
    if (pos >= _size) throw std::out_of_range("TVector::emplace");

    size_t busy_count = 0;
    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            if (busy_count == pos) {
                _data[i] = value;
                return;
            }
            ++busy_count;
        }
    }
}
template <class T>
void TVector<T>::clear() {
    for (size_t i = 0; i < _capacity; ++i) {
        _data[i] = 0;
        _states[i] = empty;
    }
    reserve(15);
    _size = 0;
    _deleted = 0;
}
template <class T>
void TVector<T>::assign(size_t count, const T& value) {
    reserve(count);
    for (size_t i = 0; i < count; ++i) {
        _data[i] = value;
        _states[i] = busy;
    }
    _size = count;
    _deleted = 0;
}

template <class T>
void TVector<T>::shrink_to_fit() {
    if (_size == _capacity) return;

    if (_size == 0) {
        free_memory();
        _capacity = 0;
        _deleted = 0;
        return;
    }
    T* new_data = new T[_capacity];
    State* new_states = new State[_capacity]{};

    size_t new_index = 0;
    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            new_data[new_index] = _data[i];
            new_states[new_index] = busy;
            ++new_index;
        }
    }

    clear_tail(new_data, new_states, new_index, _capacity);
    free_memory();

    delete[] _data;
    delete[] _states;

    _data = new_data;
    _states = new_states;
    _deleted = 0;
}

template <class T>
void TVector<T>::resize(size_t new_size) {
    if (new_size < _size) {
        for (size_t i = new_size; i < _size; ++i) {
            if (_states[i] == busy) {
                _states[i] = empty;
                ++_deleted;
            }
        }
        _size = new_size;
    } else if (new_size > _size) {
        if (new_size > _capacity) {
            reserve(new_size);
        }
        _deleted = 0;
    } else {
        return;
    }
}

template <class T>
bool TVector<T>::operator==(const TVector& other) const {
    if (_size != other._size) return false;

    size_t a = 0, b = 0;
    while (a < _capacity && b < other._capacity) {
        while (a < _capacity && _states[a] != busy) ++a;
        while (b < other._capacity && other._states[b] != busy) ++b;

        if (a < _capacity && b < other._capacity) {
            if (_data[a] != other._data[b]) return false;
            ++a; ++b;
        }
    }

    return true;
}
template <class T>
bool TVector<T>::operator!=(const TVector& other) const {
    return !(*this == other);
}
template <class T>
TVector<T>& TVector<T>::operator=(const TVector& other) {
    if (this != &other) {
        delete[] _data;
        delete[] _states;
        _capacity = other._capacity;
        _size = other._size;
        _deleted = other._deleted;
        _data = new T[_capacity];
        _states = new State[_capacity];
        my_copy(other._data, other._data + _capacity, _data);
        my_copy(other._states, other._states + _capacity, _states);
    }
    return *this;
}
template <class T>
T& TVector<T>::operator[](size_t pos) noexcept {
    size_t count = 0;
    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            if (count == pos) return _data[i];
            ++count;
        }
    }
    if (_capacity != 0) {
        return _data[0];
    }
}
template<class T>
const T& TVector<T>::operator[](size_t pos) const noexcept {
    size_t count = 0;
    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            if (count == pos) return _data[i];
            ++count;
        }
    }
    if (_capacity != 0) {
        return _data[0];
    }
}

template <class T>
inline bool TVector<T>::is_full() const noexcept {
    return _size + _deleted >= _capacity;
}

template <class T>
void TVector<T>::ensure_capacity(size_t start_index) {
    if (is_full()) {
        reserve(_capacity <= 1 ? 2 : _capacity * 3 / 2, start_index);
    }
}

template <class T>
void TVector<T>::reserve(size_t new_cap, size_t start_index) {
    if (new_cap <= _capacity) {
        return;
    }

    T* new_data = new T[new_cap];
    State* new_states = new State[new_cap]();

    size_t new_index = start_index;
    for (size_t k = 0; k < start_index; ++k) {
        new_states[k] = deleted;
    }


    for (size_t i = 0; i < _capacity; ++i) {
        if (_states[i] == busy) {
            new_data[new_index] = _data[i];
            new_states[new_index] = busy;
            ++new_index;
        }
    }

    clear_tail(new_data, new_states, _capacity + start_index, new_cap);
    free_memory();

    _data = new_data;
    _states = new_states;
    _size = new_index - start_index;
    _deleted = start_index;
    _capacity = new_cap;
}

template <class T>
void TVector<T>::clear_tail(T* data, State* states, size_t from, size_t to) {
    for (size_t k = from; k < to; ++k) {
        states[k] = empty;
    }
}

template <class T>
void TVector<T>::free_memory() {
    if (_data != nullptr) {
        delete[] _data;
        _data = nullptr;
    }
    if (_states != nullptr) {
        delete[] _states;
        _states = nullptr;
    }
}

template <class T>
template <class InputIt, class OutputIt>
void TVector<T>::my_copy(InputIt first, InputIt last, OutputIt dest) {
    while (first != last) {
        *dest = *first;
        ++first;
        ++dest;
    }
}


template <class T>
void swp(T& a, T& b) {
    T c;
    c = a;
    a = b;
    b = c;
}
template <class T>
void shuffle(TVector<T>& vec) {  // Use std::mt19937 engine(std::time(nullptr));
    size_t n = vec.size();
    if (n <= 1) {
        return;
    }
    for (size_t i = n - 1; i > 0; --i) {
        size_t j = engine() % (i + 1);
        swp(vec[i], vec[j]);
    }
}
template <class T>
void hoar_sort_rec(TVector<T>& vec, int left, int right) {
    if (left >= right) return;

    T base = vec.at((left + right) / 2);
    int i = left;
    int j = right;

    while (i <= j) {
        while (vec.at(i) < base) ++i;
        while (vec.at(j) > base) --j;

        if (i <= j) {
            swp(vec.at(i), vec.at(j));
            ++i;
            --j;
        }
    }

    if (left < j) hoar_sort_rec(vec, left, j);
    if (i < right) hoar_sort_rec(vec, i, right);
}
template <class T>
void hoar_sort(TVector<T>& vec) {
    if (vec.size() > 1)
        hoar_sort_rec(vec, 0, vec.size() - 1);
}

template <class T>
int find_first(TVector<T>& vec, const T& value) {  // Dont use at
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            return i;
        }
    }
    return -1;
}
template <class T>
int find_last(TVector<T>& vec, const T& value) {
    for (size_t i = vec.size(); i-- > 0;) {
        if (vec[i] == value) {
            return i;
        }
    }
    return -1;
}
template <class T>
TVector<size_t> find_all(TVector<T>& vec, const T& value) {
    TVector<size_t> result;
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == value) {
            result.push_back(i);
        }
    }
    return result;
}
