#pragma once
#include <pch.h>

std::ostream& operator<<(std::ostream& _os, glm::vec3 _v) {
	_os << "[" << _v[0] << ", " << _v[1] << ", " << _v[2] << "]";
	return _os;
}