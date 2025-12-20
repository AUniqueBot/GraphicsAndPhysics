#pragma once
#include <pch.h>
#include <arch/core.h>

class ShaderProgram {
/*
	@brief
		contains the shader program
	
	Not responsible for loading the program
*/

public:
	enum SHADERTYPE {
		VERTEX,
		FRAG,
		GEOMETRY,
		TESELLATION_CONTROL,
		TESSELATION_EVALUATION,
		COMPUTE,
		_COUNT
	};

public:
	void Init();
	void Load();
	void Unload();

	static GLuint LoadShader(const char* _sourceCode, SHADERTYPE _type);
	static GLuint LinkShaders(std::vector<GLuint>);

	
	void SetShaderID(GLuint _id) { m_programId = _id; };

	int ShaderID() const { return m_programId; };

private:


private:
	unsigned m_programId { 0 };
	std::bitset<_COUNT> m_shaderLoadStatus;

};

