#pragma once
#include <pch.h>
#include <variant>

constexpr unsigned C_MAX_LIGHTS { 8 };

// idk
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



struct MaterialUBO {
	glm::vec4 m_albedoColor; 
	glm::vec4 m_specularColor; 
	glm::vec4 m_emissiveColor;

	float m_metallic;
	float m_roughness;
	

};



struct LightUBO {

};