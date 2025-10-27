#pragma once

#include "pch.h"
#include <arch/resources/res_shader.h>

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

	void Init();

	void SetShaderProgram(std::shared_ptr<ShaderProgram> _shaderProg);

	int GetShader() const;


	// - material helpers -------------------------------------
	static unsigned LoadImage(std::string path, bool _hasAlpha, IMAGE_CLAMP_BEHAVIOUR _horizontal, IMAGE_CLAMP_BEHAVIOUR _vertical, FILTER_TYPE _fType);


private:
	std::shared_ptr<ShaderProgram> m_shader;
	
};