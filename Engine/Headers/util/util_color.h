#pragma once
#include <pch.h>

namespace Graphics {

	namespace Color {
		glm::u8vec4 HexToRGBA_Vec4(unsigned _color);
		glm::u8vec3 HexToRGB_Vec3(unsigned _color);
	
		

		std::vector<glm::u8vec4> ConvertData_UCharToRGBA(unsigned char* _data, int _width, int _height, int _channelCount);



	}
}

#include <util/util_color.hpp>