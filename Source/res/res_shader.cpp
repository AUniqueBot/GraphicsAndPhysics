#include "pch.h"
#include <arch/resources/res_shader.h>

void ShaderProgram::Load(std::string _vertexShader, std::string _fragShader) {
	// bwoah
}

void ShaderProgram::Compile() {
	
	// finish precompile check.
	if (!PreCompileCheck()) {
		return;
	}



	m_shaderID = glCreateProgram();
	for (int i{}; i < SHADERTYPE::_COUNT; ++i) {
		

		GLint shaderType{};
		SHADERTYPE currentType = static_cast<SHADERTYPE>(i);
		int currentShaderID = 0;
		switch (i) {
		case VERTEX:
			shaderType = GL_VERTEX_SHADER;
			break;
		case FRAG:
			shaderType = GL_FRAGMENT_SHADER;
			break;
		case GEOMETRY:
			shaderType = GL_GEOMETRY_SHADER;
			break;
		case TESELLATION_CONTROL:
			shaderType = GL_TESS_CONTROL_SHADER;
			break;
		case TESSELATION_EVALUATION:
			shaderType = GL_TESS_EVALUATION_SHADER;
			break;
		case COMPUTE:
			shaderType = GL_COMPUTE_SHADER;
			break;
		default:
			// do nothing.
			break;
		}

		currentShaderID = glCreateShader(shaderType);
		int glStatus{};
		glGetShaderiv(currentShaderID, GL_COMPILE_STATUS, &glStatus);

		LogCompileStatus(glStatus, currentShaderID);

		m_shaderLoadStatus[i] = glStatus != GL_FALSE;
		// only link if it's a success.
		if (glStatus != GL_FALSE) {
			glAttachShader(m_shaderID, currentShaderID);
			
		}



		// delete the shader.
		glDeleteShader(currentShaderID);
	}
	

	// if creation fails, delet prg.




}

void ShaderProgram::Load() {
}

void ShaderProgram::Unload() {
}





void ShaderProgram::LogCompileStatus(int _statusCode, int _shaderID) {
	char log[512];
	if (_statusCode == GL_FALSE) {
		std::cout << "ERROR: "
			<< "COMPILATION FAILED!";
	}
	glGetShaderInfoLog(_shaderID, 512, NULL, log);
}

bool ShaderProgram::PreCompileCheck() {
	if (!m_shaderMap[VERTEX].empty()) {
		std::cout << "Vertex Shader Missing" << std::endl;
	}
	if (!m_shaderMap[VERTEX].empty()) {
		std::cout << "Frag Shader Missing" << std::endl;
	}
	return
		!m_shaderMap[VERTEX].empty() &&
		!m_shaderMap[FRAG].empty();
}
