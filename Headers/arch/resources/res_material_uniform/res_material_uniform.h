#pragma once
#include <pch.h>
#include <variant>


struct UniformData  {

	using UniformType = std::variant<
		int, unsigned,
		float,
		glm::vec2, glm::vec3, glm::vec4,
		glm::mat2, glm::mat3, glm::mat4

	>;

	GLint m_uniformLocation;
	GLenum m_type;
	UniformType m_data;
};


// ./Assets/CommonUBOs.glsl // not needed anymore.
struct CameraUBO {
	glm::mat4 m_objectMatrix;
	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;
};



