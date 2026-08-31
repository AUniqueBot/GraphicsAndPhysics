#pragma once
#include <pch.h>
#include <regex>
#include <unordered_set>
#include <arch/resources/res_resource.h>
#include <arch/resources/res_resourceHandle.h>
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

	inline std::string StringifyShaderType(const ShaderType& _type) {
		return
			_type == ShaderType::VERTEX ? "Vertex" :
			_type == ShaderType::FRAG ? "Frag" :
			_type == ShaderType::GEOMETRY ? "Geometry" :
			_type == ShaderType::TESELLATION_CONTROL ? "Tesselation_Contrel" :
			_type == ShaderType::TESSELATION_EVALUATION ? "Tesselation_Evaluation" :
			_type == ShaderType::COMPUTE ? "Compute" :
			"";
	}

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

	void ShaderType(const ShaderConstants::ShaderType& _type);
	const ShaderConstants::ShaderType& ShaderType() const;

	void SetShaderCode(const std::string& _shaderCode);
	const std::string& GetShaderCode() const;

	const GLuint& GetShaderID() const;


	void Build(bool _showDebugMessages = true);
	void Destroy() override;
private:
	GLuint m_shaderId			{ ShaderConstants::C_INVALIDSHADERID };
	std::string m_shaderCode	{};
	ShaderConstants::ShaderType m_shaderType {};
	bool m_shaderIsBuilt		{ false };

public:
	INSPECTABLE_DECLAREPROPS(Shader);
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

	void SetShader(const Shader& _shader); // uses the shader's internal type. wrapper of explicit version.
	void SetShader(const Shader& _shader, const ShaderConstants::ShaderType& _type);
	void SetShader(const GLuint& _shaderId, const ShaderConstants::ShaderType& _type);


	bool IsValid() const;
	bool IsValidComputeShader() const;
	bool IsValidRenderShader() const;

	void SetShaderProgramID(GLuint _id);
	int GetShaderProgramID() const;

	void Build();
	void Destroy() override;

private:

	std::deque<GLuint> GetShaderIDList() const;
private:
	GLuint m_shaderProgramId								{ 0 };
	SparseSet<ShaderConstants::ShaderType, GLuint> m_shaderIds;

};

// ------------------------------------------------------------------------------

using ShaderHandle = ResourceHandleT<Shader>;
using ShaderProgramHandle = ResourceHandleT<ShaderProgram>;
