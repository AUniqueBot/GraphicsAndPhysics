#pragma once
#include <pch.h>
#include <arch/core.h>
#include <regex>
#include <unordered_set>



class ShaderProgram : public Resource<ShaderProgram> {
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

	static std::string ParseShaderCode(
		const std::string& _sourceCodePath
	);
	static GLuint LoadShader(const char* _sourceCode, SHADERTYPE _type);
	static GLuint GenerateShaderProgram(std::vector<GLuint>);

	
	void SetShaderID(GLuint _id) { m_programId = _id; };

	int ShaderID() const { return m_programId; };

	// a list of inputs from vertex buffers, uniforms from (???), and outputs from render targets
	std::string GenerateShader();


private:
	static std::string ParseShaderCode_Internal(
		const std::string& filePath,
		std::unordered_set<std::string>& visited
	);


private:
	unsigned m_programId { 0 };
	std::bitset<_COUNT> m_shaderLoadStatus;

};




