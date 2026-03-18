#pragma once
#include <pch.h>
#include <regex>
#include <unordered_set>
#include <arch/resources/res_resource.h>
#include <arch/datatypes/type_sparseSet.h>




namespace ShaderConstants {
	inline constexpr GLuint C_INVALIDSHADERID				{ 0 };
	enum class ShaderType {
		VERTEX,
		FRAG,
		GEOMETRY,
		TESELLATION_CONTROL,
		TESSELATION_EVALUATION,
		COMPUTE,
		_COUNT
	};
}

namespace ShaderUtilFunctions {

	std::string ParseShaderCode(const std::string& _sourceCodePath);
	GLuint CompileShader(const char* _sourceCode, ShaderConstants::ShaderType _type);
	GLuint BuildShaderProgram(std::vector<GLuint>);
}


std::ostream& operator<<(std::ostream& _os, ShaderConstants::ShaderType _type);


class Shader : public Resource<Shader> {
public:

public:

	void SetShaderType(ShaderConstants::ShaderType _type);
	const ShaderConstants::ShaderType& GetShaderType() const;

	void SetShaderCode(const std::string& _shaderCode);
	const std::string& GetShaderCode() const;

	const GLuint& GetShaderID() const;


	void Build(bool _showDebugMessages = true);
	void Destroy();
private:
	std::string m_shaderName	{};
	GLuint m_shaderId			{ ShaderConstants::C_INVALIDSHADERID };
	std::string m_shaderCode	{};
	ShaderConstants::ShaderType m_shaderType		{};
	bool m_shaderIsBuilt		{ false };
};



class ShaderProgram : public Resource<ShaderProgram> {
/*
	@brief
		contains the shader program
	Not responsible for loading the program
*/
public:
	void Init();
	void Load();
	void Unload();

	void SetShader(Shader _shader, ShaderConstants::ShaderType);

	void SetShaderProgramID(GLuint _id);
	int GetShaderProgramID() const;

	void Build();
	void Destroy();

private:
	std::vector<GLuint> GetShaderVectorList() const;
private:
	GLuint m_programId												{ 0 };
	SparseSet<ShaderConstants::ShaderType, Shader> m_shaderList;
};




