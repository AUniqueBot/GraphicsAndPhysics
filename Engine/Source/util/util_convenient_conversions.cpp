#include <pch.h>
#include <util/util_convenient_conversions.h>

namespace Color {
	glm::vec4 HexToVec4F(unsigned _hexVal) {
		return glm::vec4(
			(float)(_hexVal >> (8 * 3) & 0xff) / 0xff,
			(float)(_hexVal >> (8 * 2) & 0xff) / 0xff,
			(float)(_hexVal >> (8 * 1) & 0xff) / 0xff,
			(float)(_hexVal >> (8 * 0) & 0xff) / 0xff
		);
	}

	glm::vec3 HexToVec3F(unsigned _hexVal) {
		return glm::vec3(
			(float)(_hexVal >> (8 * 2) & 0xff) / 0xff,
			(float)(_hexVal >> (8 * 1) & 0xff) / 0xff,
			(float)(_hexVal >> (8 * 0) & 0xff) / 0xff
		);
	}

	unsigned Vec4FToHex(glm::vec4 _col) {
		// [0xrrggbbaa]
		unsigned val = 
			static_cast<unsigned>(_col.r * (0xff << 8 * 3)) |  
			static_cast<unsigned>(_col.g * (0xff << 8 * 2)) |  
			static_cast<unsigned>(_col.b * (0xff << 8 * 1)) |  
			static_cast<unsigned>(_col.a * (0xff << 8 * 0))
			;
		return val;
	}

	unsigned Vec3FToHex(glm::vec3 _col) {
		// [0xrrggbb]
		unsigned val =
			static_cast<unsigned>(_col.r * (0xff << 8 * 2)) |
			static_cast<unsigned>(_col.g * (0xff << 8 * 1)) |
			static_cast<unsigned>(_col.b * (0xff << 8 * 0)) 
			;
		return val;
	}
}