#pragma once

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



glm::vec4 HexToVec4(unsigned _hexVal) {
	return glm::vec4(
		(float)(_hexVal >> (8 * 3) & 0xff) / 0xff,
		(float)(_hexVal >> (8 * 2) & 0xff) / 0xff,
		(float)(_hexVal >> (8 * 1) & 0xff) / 0xff,
		(float)(_hexVal >> (8 * 0) & 0xff) / 0xff
	);
}

glm::vec3 HexToVec3(unsigned _hexVal) {
	return glm::vec3(
		(float)(_hexVal >> (8 * 2) & 0xff) / 0xff,
		(float)(_hexVal >> (8 * 1) & 0xff) / 0xff,
		(float)(_hexVal >> (8 * 0) & 0xff) / 0xff
	);
}