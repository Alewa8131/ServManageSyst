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
	inline T* data() noexcept { return _data; }
	inline size_t size() const noexcept { return _size; }
	inline size_t capacity() const noexcept { return _capacity; }
	inline T& front() noexcept { return _data[0]; }
	inline T& back() noexcept { return _data[_size - 1]; }
	inline T* begin() noexcept { return _data; }
	inline T* end() noexcept { return _data + _size; }

	void push_front(const T& value) {
		ensure_capacity();

		// ≈сли первый элемент не зан€т Ч вставл€ем без сдвига
		if (_states[0] != busy) {
			_data[0] = value;
			if (_states[0] == deleted) {
				--_deleted;
			}
			_states[0] = busy;
			++_size;
			return;
		}

		// »щем позицию куда можно сдвинуть (последн€€ пуста€ или удалЄнна€)
		size_t target = _capacity;
		for (size_t i = _capacity; i-- > 0;) {
			if (_states[i] != busy) {
				target = i;
				break;
			}
		}

		// —двигаем элементы назад до найденной позиции
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

		// ≈сли не получилось Ч вставл€ем в первое пустое или удалЄнное место в конце
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
		//ensure_capacity(); // √лупа€ проверка т. к. убираютс€ пустые элементы и нет смысла на них провер€ть

		// »щем физический индекс логической позиции pos
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
		bool can_shift_left = (_states[left - 1] != busy && left - 1 < physical_index);

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
	void ensure_capacity() {
		shrink_to_fit();
		if (_size + _deleted >= _capacity) {
			reserve(_capacity == 0 ? 1 : _capacity * 3 / 2);
		}
	}
	void reserve(size_t new_cap) {
		if (new_cap <= _capacity) {
			new_cap = _capacity + 1;
		}

		T* new_data = new T[new_cap];
		State* new_states = new State[new_cap]();

		size_t new_size = 0;
		for (size_t i = 0; i < _size + _deleted; ++i) {
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

	inline bool is_full() const noexcept {
		return _size + _deleted >= _capacity;
	}
};