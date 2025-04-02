#pragma once

enum State { empty, busy, deleted };
template <class T>
class TVector {
	T* _data;
	size_t _capacity;
	size_t _size;
public:
	TVector();
	TVector(int size);
	TVector(const TVector&);
private:
	size_t _deleted;
	State* _states;
};