#pragma once

#include "pch.h"
#include <arch/resources/res_shader.h>

class Material {
public:
	using SHADERTYPE = ShaderProgram::SHADERTYPE; // alias the name

public:

	void Init();

	void SetShaderProgram(std::shared_ptr<ShaderProgram> _shaderProg);

	int GetShader() const;



private:
	std::shared_ptr<ShaderProgram> m_shader;
	
};