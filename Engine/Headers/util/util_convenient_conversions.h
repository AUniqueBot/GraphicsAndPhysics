#pragma once
#include <glm/common.hpp>
// hex to vec conversions.

namespace Color {

	glm::vec4 HexToVec4F(unsigned _hexVal);
	glm::vec3 HexToVec3F(unsigned _hexVal);
	unsigned Vec4FToHex(glm::vec4 _col);
	unsigned Vec3FToHex(glm::vec3 _col);


	template <typename T>
	inline T HexToVec(unsigned _hexVal);
	template <>
	inline glm::vec3 HexToVec(unsigned _hexVal) {
		return HexToVec3F(_hexVal);
	}
	template <>
	inline glm::vec4 HexToVec(unsigned _hexVal) {
		return HexToVec4F(_hexVal);
	}

	template <typename T>
	inline unsigned VecToHex(T _col);
	template <>
	inline unsigned VecToHex(glm::vec4 _col) {
		return Vec4FToHex(_col);
	}

	template <>
	inline unsigned VecToHex(glm::vec3 _col) {
		return Vec3FToHex(_col);
	}
}
