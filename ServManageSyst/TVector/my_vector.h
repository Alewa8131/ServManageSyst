// Copyright 2025 Alewa8131
#pragma once
#include <iostream>
#include <initializer_list>
#include <cstddef>

enum State { empty, busy, deleted };
template <class T>
class TVector {
    T* _data = nullptr;
    State* _states = nullptr;
    size_t _capacity = 0;
    size_t _size = 0;
    size_t _deleted = 0;

 public:
     TVector() = default;

     explicit TVector(size_t size) : _data(new T[size]), _capacity(size), _size(size),
		_states(new State[size]) {
		std::fill_n(_states, size, busy);
	 }
     TVector(std::initializer_list<T> init) :
		_data(new T[init.size()]),
		_capacity(init.size()),
		_size(init.size()),
		_states(new State[init.size()]) {
		std::copy(init.begin(), init.end(), _data);
		std::fill_n(_states, _size, busy);
	 }

     template <size_t N>
	 explicit TVector(const T(&arr)[N]) : TVector(N) {
		std::copy(arr, arr + N, _data);
	 }
     TVector(const T* arr, size_t size) : TVector(size) {
		std::copy(arr, arr + size, _data);
	 }
     TVector(const TVector& other) : 
		_data(new T[other._capacity]), _states(new State[other._capacity]),
		_capacity(other._capacity), _size(other._size), _deleted(other._deleted) {
		std::copy(other._data, other._data + other._capacity, _data);
		std::copy(other._states, other._states + other._capacity, _states);
	 }

     ~TVector() {
		delete[] _data;
		delete[] _states;
	 }


     inline bool is_empty() const noexcept { return _size == 0; }
     inline size_t size() const noexcept { return _size; }
     inline size_t capacity() const noexcept { return _capacity; }
     inline T* data() noexcept { return _data; }
     inline T* begin() noexcept { return _data; }
     inline T* end() noexcept { return _data + _capacity; }
     inline T& front() noexcept { return at(0); }
     T& back() noexcept {
		for (size_t i = _capacity; i-- > 0;)
			if (_states[i] == busy)
				return _data[i];
		throw std::out_of_range("TVector::back - empty vector");
	 }
     T& at(size_t pos) {
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
     
     void push_front(const T& value) {
		ensure_capacity();

		// Если первый элемент не занят — вставляем без сдвига
		if (_states[0] != busy) {
			_data[0] = value;
			if (_states[0] == deleted) {
				--_deleted;
			}
			_states[0] = busy;
			++_size;
			return;
		}

		// Ищем позицию куда можно сдвинуть (последняя пустая или удалённая)
		size_t target = _capacity;
		for (size_t i = _capacity; i-- > 0;) {
			if (_states[i] != busy) {
				target = i;
				break;
			}
		}

		// Сдвигаем элементы назад до найденной позиции
		for (size_t i = target; i > 0; --i) {
			_data[i] = _data[i - 1];
			_states[i] = _states[i - 1];
		}

		_data[0] = value;
		_states[0] = busy;
		++_size;
	 }
     void push_back(const T& value) {
		ensure_capacity();

		for (size_t i = _capacity; i-- > 0;) {
			if (_states[i] == busy) {
				if (i + 1 < _capacity && _states[i + 1] != busy) {
					_data[i + 1] = value;
					_states[i + 1] = busy;
					++_size;
					if (_states[i + 1] == deleted) --_deleted;
					return;
				}
				break;
			}
		}

		if (_size == 0 && _capacity > 0 && _states[0] != busy) {
			_data[0] = value;
			_states[0] = busy;
			++_size;
			return;
		}

		// Если не получилось — вставляем в первое пустое или удалённое место в конце
		for (size_t i = _capacity; i-- > 0;) {
			if (_states[i] != busy) {
				_data[i] = value;
				_states[i] = busy;
				++_size;
				if (_states[i] == deleted) --_deleted;
				return;
			}
		}
	 }
     void insert(size_t pos, const T& value) {
		ensure_capacity();  // Глупая проверка 
		// т. к. убираются пустые элементы и нет смысла на них проверять
		// reserve(_capacity + 1); // Неявное +1 к размеру _capacity

		// Ищем физический индекс логической позиции pos
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
     
     
     void pop_front() {
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
     void pop_back() {
		if (_size == 0) return;

		for (size_t i = _capacity; i-- > 0;) {
			if (_states[i] == busy) {
				_states[i] = deleted;
				++_deleted;
				--_size;
				return;
			}
		}
	 }
     void erase(size_t pos) {
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
     
     void emplace(size_t pos, const T& value) {
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
     void clear() {
     	for (size_t i = 0; i < _capacity; ++i) {
     		_states[i] = empty;
     	}
     	_size = 0;
     	_deleted = 0;
     }
     void assign(size_t count, const T& value) {
     	clear();
     	reserve(count);
     	for (size_t i = 0; i < count; ++i) {
     		_data[i] = value;
     		_states[i] = busy;
     	}
     	_size = count;
     	_deleted = 0;
     }
     
     
     void shrink_to_fit() {
		if (_size == _capacity) return;

		T* new_data = new T[_size];
		State* new_states = new State[_size]{};

		size_t new_index = 0;
		for (size_t i = 0; i < _capacity; ++i) {
			if (_states[i] == busy) {
				new_data[new_index] = _data[i];
				new_states[new_index] = busy;
				++new_index;
			}
		}

		delete[] _data;
		delete[] _states;

		_data = new_data;
		_states = new_states;
		_capacity = _size;
		_deleted = 0;
	 }
     
     void resize(size_t new_size) {
		if (new_size < _size) {
			for (size_t i = new_size; i < _size; ++i) {
				if (_states[i] == busy) {
					_states[i] = deleted;
					++_deleted;
				}
			}
			_size = new_size;
		} else if (new_size > _size) {
			if (new_size > _capacity) {
				reserve(new_size);
			}
			for (size_t i = _size; i < new_size; ++i) {
				_data[i] = T{};
				_states[i] = busy;
			}
			_size = new_size;
			_deleted = 0;
		}
	 }
     
     bool operator==(const TVector& other) const {
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
     bool operator!=(const TVector& other) const {
		return !(*this == other);
	 }
     TVector& operator=(const TVector& other) {
		if (this != &other) {
			delete[] _data;
			delete[] _states;
			_capacity = other._capacity;
			_size = other._size;
			_deleted = other._deleted;
			_data = new T[_capacity];
			_states = new State[_capacity];
			std::copy(other._data, other._data + _capacity, _data);
			std::copy(other._states, other._states + _capacity, _states);
		}
		return *this;
	 }
     T& operator[](size_t pos) noexcept {
		size_t count = 0;
		for (size_t i = 0; i < _capacity; ++i) {
			if (_states[i] == busy) {
				if (count == pos) return _data[i];
				++count;
			}
		}
		throw std::out_of_range("TVector::operator[]: invalid index");
	 }
     
     
     template <class T>
     friend void shuffle(TVector<T>& vec) {
		for (size_t i = vec._size - 1; i > 0; --i) {
			size_t j = rand() % (i + 1);
			if (i != j) {
				std::swap(vec._data[i], vec._data[j]);
				std::swap(vec._states[i], vec._states[j]);
			}
		}
	 }
     
     template <class T>
     friend void sort(TVector<T>& vec) {
		for (size_t i = 0; i < vec._size - 1; ++i) {
			for (size_t j = i + 1; j < vec._size; ++j) {
				if (vec._data[i] > vec._data[j]) {
					std::swap(vec._data[i], vec._data[j]);
					std::swap(vec._states[i], vec._states[j]);
				}
			}
		}
	 }


 private:
		inline bool is_full() const noexcept { return _size + _deleted >= _capacity; }
		void ensure_capacity() {
			if (is_full()) {
				reserve(_capacity <= 1 ? 2 : _capacity * 3 / 2);
			}
		}
		void reserve(size_t new_cap) {
			if (new_cap <= _capacity) {
				return;
			}

			T* new_data = new T[new_cap];
			State* new_states = new State[new_cap]();

			size_t j = 0;
			for (size_t i = 0; i < _capacity; ++i) {
				if (_states[i] == busy) {
					new_data[j] = _data[i];
					new_states[j] = busy;
					++j;
				}
			}

			delete[] _data;
			delete[] _states;

			_data = new_data;
			_states = new_states;
			_size = j;
			_deleted = 0;
			_capacity = new_cap;
		}
};
