#pragma once
#include <cstddef>

enum State { empty, busy, deleted };
template <class T>
class TVector {
	T* _data = nullptr;
	size_t _capacity = 0;
	size_t _size = 0;
	size_t _deleted = 0;
	State* _states = nullptr;

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
		_states(new State[init.size()])
	{
		std::copy(init.begin(), init.end(), _data);
		std::fill_n(_states, _size, busy);
	}

	template <size_t N>
	TVector(const T(&arr)[N]) : TVector(N) {
		std::copy(arr, arr + N, _data);
	}
	TVector(const T* arr, size_t size) : TVector(size) {
		std::copy(arr, arr + size, _data);
	}

	TVector(const TVector& other) : TVector(other._size) {
		std::copy(other._data, other._data + other._size, _data);
		std::copy(other._states, other._states + other._size, _states);
		_deleted = other._deleted;
	}

	~TVector() {
		delete[] _data;
		delete[] _states;
	}


	inline bool is_empty() const noexcept {
		return _size == 0;
	}
	T* data() noexcept { return _data; }
	size_t size() const noexcept { return _size; }
	size_t capacity() const noexcept { return _capacity; }
	T& front() noexcept { return _data[0]; }
	T& back() noexcept { return _data[_size - 1]; }
	T* begin() noexcept { return _data; }
	T* end() noexcept { return _data + _size; }

	void push_front(const T& value) {
		if (_size + _deleted >= _capacity) {
			reserve(_capacity == 0 ? 1 : _capacity * 3 / 2);
		}

		if (_size > 0) {
			std::copy_backward(_data, _data + _size, _data + _size + 1);
			std::copy_backward(_states, _states + _size, _states + _size + 1);
		}

		_data[0] = value;
		_states[0] = busy;
		++_size;
	}

	void push_back(const T& value) {
		for (size_t i = 0; i < _capacity; ++i) {
			if (_states[i] == deleted) {
				_data[i] = value;
				_states[i] = busy;
				++_size;
				--_deleted;
				return;
			}
		}

		if (_size >= _capacity) {
			reserve(_capacity == 0 ? 1 : _capacity * 3/2);
		}

		_data[_size] = value;
		_states[_size] = busy;
		++_size;
	}


	void insert();

	void pop_front() {
		if (_size == 0) return;

		for (size_t i = 0; i < _size + _deleted; ++i) {
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

		for (size_t i = _size + _deleted; i > 0; i--) {
			if (_states[i] == busy) {
				_states[i] = deleted;
				++_deleted;
				--_size;
				return;
			}
		}
	}
	void erase();

	void emplace();
	void assign();
	T& at(size_t pos) {
		if (pos >= _size) throw std::out_of_range("TVector::at");
		return _data[pos];
	}
	void clear();

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
	void reserve(size_t new_cap) {
		if (new_cap <= _capacity) return;

		T* new_data = new T[new_cap];
		State* new_states = new State[new_cap]();

		size_t new_size = 0;
		for (size_t i = 0; i < _size+_deleted; ++i) {
			if (_states[i] == busy) {
				new_data[new_size] = _data[i];
				new_states[new_size] = busy;
				++new_size;
			}
		}

		delete[] _data;
		delete[] _states;

		_data = new_data;
		_states = new_states;
		_size = new_size;
		_deleted = 0;    
		_capacity = new_cap;
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
		}
		else if (new_size > _size) {

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
		return std::equal(_data, _data + _size, other._data);
	}
	bool operator!= (const TVector& other) const;
	TVector& operator= (const TVector& other);

	T& operator[] (size_t pos) noexcept {
		size_t active_pos = 0;
		for (size_t i = 0; i < _size; ++i) {
			if (_states[i] == busy) {
				if (active_pos == pos) {
					return _data[i];
				}
				++active_pos;
			}
		}
	}

	inline bool is_full() const noexcept;
};