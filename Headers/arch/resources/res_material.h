#pragma once

#include <pch.h>
#include <arch/resources/res_shader.h>

// - constants -------------------------

constexpr const char* OBJECT_MATRIX		{ "u_objectMtx" };
constexpr const char* CAMERA_MATRIX		{ "u_cameraMtx" };
constexpr const char* PROJECTION_MATRIX	{ "u_projectionMtx" };


// - class -----------------------------
class Material {
public:
	using SHADERTYPE = ShaderProgram::SHADERTYPE; // alias the name

public:
	enum IMAGE_CLAMP_BEHAVIOUR {
		REPEAT,
		MIRROR_REPEAT,
		TO_EDGE,
		TO_BORDER
	};

	enum FILTER_TYPE {
		LINEAR,
		NEAREST
	};



public:

	virtual void Init();

	void SetShaderProgram(std::shared_ptr<ShaderProgram> _shaderProg);

	int GetShader() const;

	virtual void Render(
		const glm::mat4& _objectMatrix, 
		const glm::mat4& _projectionMatrix, 
		const glm::mat4& _cameraMatrix
		) const {};


	// - material helpers -------------------------------------
	static unsigned LoadImage(std::string path, bool _hasAlpha, IMAGE_CLAMP_BEHAVIOUR _horizontal, IMAGE_CLAMP_BEHAVIOUR _vertical, FILTER_TYPE _fType);


protected:
	void InitUniformLocations();
	
	std::shared_ptr<ShaderProgram> m_shader;
	std::map<std::string, GLuint> m_uniformLocations;
	

};