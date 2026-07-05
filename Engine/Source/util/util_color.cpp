#include <util/util_color.h>

using namespace Graphics;
namespace Color {
	glm::u8vec4 HexToRGBA_Vec4(unsigned _color) {
		unsigned char r{}, g{}, b{}, a{};
		r = (_color >> 24) & 0xff;
		g = (_color >> 16) & 0xff;
		b = (_color >> 8) & 0xff;
		a = (_color >> 0) & 0xff;
		return { r, g, b, a };
	}

	glm::u8vec3 HexToRGB_Vec3(unsigned _color) {
		unsigned char r{}, g{}, b{};
		r = (_color >> 16) & 0xff;
		g = (_color >> 8) & 0xff;
		b = (_color >> 0) & 0xff;
		return { r, g, b };
	}

	std::vector<glm::u8vec4> ConvertData_UCharToRGBA(unsigned char* _data, int _width, int _height, int _channelCount) {
		// run multithreaded approach.
		std::vector<glm::u8vec4> converted((_width * _height), {1, 1, 1, 1});
		for (int i{}; i < _width * _height; ) {
			converted[i].r = _data[i * _channelCount];
			if (_channelCount > 1) converted[i].g = _data[i * _channelCount + 1];
			if (_channelCount > 2) converted[i].b = _data[i * _channelCount + 2];
			if (_channelCount > 3) converted[i].a = _data[i * _channelCount + 3];
		}
		return converted;
	}



}
