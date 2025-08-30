#pragma once
#include <glm/glm.hpp>
#include <iostream>



// printing vectors
template <typename T, size_t N, glm::qualifier Q>
std::ostream& operator<<(std::ostream& _os, glm::vec<N, T, Q> _value) {
	_os << "[";
	for (unsigned i{}; i < N; ++i) {
		_os << _value[i] << (i != N - 1 ? ", " : "");
	}
	_os << ']';
	return _os;
}


// printing matrices.
template <typename T, size_t X, size_t Y, glm::qualifier Q>
std::ostream& operator<<(std::ostream & _os, glm::mat<X, Y, T, Q>_value) {
	for (size_t c = 0; c < X; ++c) {
		for (size_t r = 0; r < Y; ++r) {
			_os << '[' << _value[c][r] << ']';
		}
		_os << '\n';
	}
	return _os;
}