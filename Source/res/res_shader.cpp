#include <pch.h>
#include <filesystem>
#include <arch/resources/res_shader.h>
#include <util/util_logging.h>

static std::string GetRawText(std::string _pathToFile) {
	std::stringstream toRet;
	std::ifstream ifs{ _pathToFile };
	if (!ifs.good()) return std::string{};
	toRet << ifs.rdbuf();
	return toRet.str();
}







void ShaderProgram::Init() {

}



void ShaderProgram::Load() {
	
}

void ShaderProgram::Unload() {

}



GLuint ShaderProgram::LinkShaders(std::vector<GLuint> _shaderList) {
	// go through shaders and link
	GLuint programId = glCreateProgram();
	for (GLuint& shaderId : _shaderList) {
		if (!shaderId) {
			// other logic here.
			continue;
		}
		glAttachShader(programId, shaderId);
	}
	glLinkProgram(programId);
	


	GLint status{};
	glGetProgramiv(programId, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		LOG_WARN("Program Linking Failed");
		glDeleteProgram(programId);
		return 0;
	}

	LOG_INFO("Program Generation Complete");
	return programId;
}




GLuint ShaderProgram::LoadShader(const char* _sourceCode, SHADERTYPE _type) {
	// - reject cases -------------------------------
	if (_type > FRAG) { // if (_type > _COUNT) { // once full implementation is complete.
		LOG_WARN("Unsupported shader type");
		return 0;
	}

	// - source code handle -------------------------
	const char* sourceTextPointer = _sourceCode;
	
	
	// - type casting -------------------------------
	GLuint shaderType = 0;
	switch (_type) {
	case VERTEX:
		shaderType = GL_VERTEX_SHADER;
		break;
	case FRAG:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	//case GEOMETRY:
	//	shaderType = GL_GEOMETRY_SHADER;
	//	break;
	//case TESELLATION_CONTROL:
	//	shaderType = GL_TESS_CONTROL_SHADER;
	//	break;
	//case TESSELATION_EVALUATION:
	//	shaderType = GL_TESS_EVALUATION_SHADER;
	//	break;
	//case COMPUTE:
	//	shaderType = GL_COMPUTE_SHADER;
	//	break;
	default:
		shaderType = 0;
		break;
	}
	

	// - shader generation --------------------------
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &sourceTextPointer, NULL);
	glCompileShader(shaderId);

	// - error handling -----------------------------
	GLint status{};
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		LOG_ERROR("Shader Compilation Failed");
		glDeleteShader(shaderId);
		return 0;
	}
	LOG_INFO("Program Generation Complete");
	return shaderId;
}




