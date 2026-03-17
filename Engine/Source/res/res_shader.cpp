#include <pch.h>
#include <filesystem>
#include <arch/resources/res_shader.h>
#include <util/util_logging.h>
#include <util/util_serialisation.h>



static std::string GetRawText(std::string _pathToFile) {
	std::stringstream toRet;
	std::ifstream ifs{ _pathToFile };
	if (!ifs.good()) return std::string{};
	toRet << ifs.rdbuf();
	return toRet.str();
}

void Shader::SetShaderType(ShaderConstants::ShaderType _type) {
	m_shaderType = _type;
}

const ShaderConstants::ShaderType& Shader::GetShaderType() const {
	return m_shaderType;
}

void Shader::SetShaderCode(const std::string& _shaderCode) {
	m_shaderCode = _shaderCode;
}

const std::string& Shader::GetShaderCode() const {
	return m_shaderCode;
}

const GLuint& Shader::GetShaderID() const {
	return m_shaderId;
}

void Shader::Build(bool _showDebugMessages) {
	if (m_shaderIsBuilt) {
		Destroy();
	}

	// - reject cases -------------------------------
	// if (_type > _COUNT) { // once full implementation is complete.
	if (m_shaderType > ShaderConstants::ShaderType::FRAG) {
		if (_showDebugMessages) LOG_WARN("Unsupported shader type");
		return;
	}

	// - type casting -------------------------------
	GLuint shaderType{};
	switch (m_shaderType) {
	case ShaderConstants::ShaderType::VERTEX:
		shaderType = GL_VERTEX_SHADER;
		break;
	case ShaderConstants::ShaderType::FRAG:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case ShaderConstants::ShaderType::GEOMETRY:
		shaderType = GL_GEOMETRY_SHADER;
		break;
	case ShaderConstants::ShaderType::TESELLATION_CONTROL:
		shaderType = GL_TESS_CONTROL_SHADER;
		break;
	case ShaderConstants::ShaderType::TESSELATION_EVALUATION:
		shaderType = GL_TESS_EVALUATION_SHADER;
		break;
	case ShaderConstants::ShaderType::COMPUTE:
		shaderType = GL_COMPUTE_SHADER;
		break;
	default:
		shaderType = 0;
		break;
	}


	// - shader generation --------------------------
	const char* sourceTextPointer		{ m_shaderCode.c_str() };
	m_shaderId = glCreateShader(shaderType) ;
	glShaderSource(m_shaderId, 1, &sourceTextPointer, NULL);
	glCompileShader(m_shaderId);

	// - error handling -----------------------------
	GLint status{};
	glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &status);
	bool shaderBuildStatus{ status != GL_FALSE };
	if (!shaderBuildStatus) {
		LOG_ERROR("Shader Compilation Failed");
		if (_showDebugMessages) {
			// log errors here.
			int maxLength = 0;
			glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &maxLength);

			std::string log(maxLength, '\0');
			glGetShaderInfoLog(m_shaderId, maxLength, &maxLength, log.data());
			LOG_ERROR("Shader Compilation Error: " << log);
		}
		Destroy();
	}
	else {
		LOG_INFO("Program Generation Complete");
		m_shaderIsBuilt = true;
	}
}

void Shader::Destroy() {
	if (!m_shaderIsBuilt) return;
	glDeleteShader(m_shaderId);
	m_shaderId = ShaderConstants::C_INVALIDSHADERID;
}


// - material side --------------------------------------------------------------------------------
void ShaderProgram::Init() {

}

void ShaderProgram::Load() {
	
}

void ShaderProgram::Unload() {

}

void ShaderProgram::SetShader(Shader _shader, ShaderConstants::ShaderType _type) {
	SparseSetView<Shader> element{ m_shaderList.At(_type) };
	if (static_cast<bool>(element)) {
		*element = _shader;
	}
	else {
		m_shaderList.Add(std::move(_shader), _type);
	}
}

void ShaderProgram::SetShaderProgramID(GLuint _id) { 
	m_programId = _id; 
};

int ShaderProgram::GetShaderProgramID() const { 
	return m_programId; 
};

void ShaderProgram::Build() {

	std::vector<GLuint> shaderList{ GetShaderVectorList() };
	
	if (shaderList.empty()) {
		LOG_ERROR("Not enough shaders provided or missing vertex/fragment shader.");
		return;
	}

	GLuint prg{ glCreateProgram() };
	for (const GLuint& shader : shaderList) {
		glAttachShader(prg, shader);
	}

	glLinkProgram(prg);
	GLint status{};
	glGetProgramiv(prg, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		GLint logLen{};
		glGetProgramiv(prg, GL_INFO_LOG_LENGTH, &logLen);
		std::string logMessage(logLen, '\0');
		glGetProgramInfoLog(prg, logLen, &logLen, logMessage.data());
		LOG_ERROR("Link Error: " << logMessage);
		glDeleteProgram(prg);
		return;
	}
	LOG_INFO("Program Generation Complete.");

	if (m_programId && prg != 0) {
		Destroy();
	}
	m_programId = prg;
}
void ShaderProgram::Destroy(){
	glDeleteProgram(m_programId);
	m_programId = 0;
}


std::vector<GLuint> ShaderProgram::GetShaderVectorList() const {
	std::vector<GLuint> shaderList(m_shaderList.size());
	bool hasVertexShader{};
	bool hasFragShader{};

	for (const auto& shader : m_shaderList) {
		GLuint shaderId{ shader.GetShaderID() };
		
		if (shaderId == 0) {
			LOG_INFO("Ignoring invalid shader ID provided for " << shader.GetShaderType() << " shader");
			continue;
		}

		if (shader.GetShaderType() == ShaderConstants::ShaderType::VERTEX) hasVertexShader = true;
		if (shader.GetShaderType() == ShaderConstants::ShaderType::FRAG) hasFragShader = true;
		shaderList.push_back(shader.GetShaderID());
	}

	if (!hasVertexShader || !hasFragShader) {
		std::string missingShaders{};
		if (!hasVertexShader) missingShaders += "VERTEX";
		if (!hasFragShader) missingShaders += std::string(missingShaders.size() > 0 ? ", " : "") + "FRAGMENT";
		LOG_ERROR("Missing shader types: [" << missingShaders << "]");
		return {};
	}
	
	return shaderList;
}


std::ostream& operator<<(std::ostream& _os, ShaderConstants::ShaderType _type) {
	std::string name{};
	using namespace ShaderConstants;
	switch (_type) {
	case ShaderType::VERTEX:
		name = "VERTEX";
		break;
	case ShaderType::FRAG:
		name = "FRAG";
		break;
	case ShaderType::GEOMETRY:
		name = "GEOMETRY";
		break;
	case ShaderType::TESELLATION_CONTROL:
		name = "TESELLATION_CONTROL";
		break;
	case ShaderType::TESSELATION_EVALUATION:
		name = "TESELLATION_EVALUATION";
		break;
	case ShaderType::COMPUTE:
		name = "COMPUTE";
		break;
	}
	_os << name;
	return _os;
}


// - static functions -----------------------------------------------------------------------------

// - static functions -----------------------------------------------------------------------------
GLuint ShaderUtilFunctions::CompileShader(const char* _sourceCode, ShaderConstants::ShaderType _type) {
	// - reject cases -------------------------------
	if (_type > ShaderConstants::ShaderType::FRAG) { // if (_type > _COUNT) { // once full implementation is complete.
		LOG_WARN("Unsupported shader type");
		return 0;
	}

	// - source code handle -------------------------
	const char* sourceTextPointer = _sourceCode;


	// - type casting -------------------------------
	GLuint shaderType = 0;
	switch (_type) {
	case ShaderConstants::ShaderType::VERTEX:
		shaderType = GL_VERTEX_SHADER;
		break;
	case ShaderConstants::ShaderType::FRAG:
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

		int maxLength{};
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::string log(maxLength, '\0');
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, log.data());
		LOG_ERROR("Shader Compilation Error: " << log);

		glDeleteShader(shaderId);
		return 0;
	}
	LOG_INFO("Program Generation Complete");
	return shaderId;
}

GLuint ShaderUtilFunctions::BuildShaderProgram(std::vector<GLuint> _shaderList) {
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

	// cleanup
	for (GLuint& shaderId : _shaderList) {
		glDetachShader(programId, shaderId);
		glDeleteShader(shaderId);
	}

	if (status == GL_FALSE) {
		LOG_WARN("Program Linking Failed");

		int maxLength{};
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength);

		std::string log(maxLength, '\0');
		glGetProgramInfoLog(programId, maxLength, &maxLength, log.data());
		LOG_ERROR("Program Link Error:\n" << log);

		glDeleteProgram(programId);
		return 0;
	}

	LOG_INFO("Program Generation Complete");
	return programId;
}

static std::string ParseShaderCode_Internal(const std::string& filePath, std::unordered_set<std::string>& visited) {
	if (visited.contains(filePath)) {
		return "// Skipping already included: " + filePath + "\n";
	}

	visited.insert(filePath);

	std::string sourceCode = FileReading::GetRawTextFromFile(filePath);
	std::stringstream parsedCode;
	std::istringstream stream(sourceCode);
	std::string line;

	std::regex includeRegex(R"(^\s*#include\s+[\"<](.+)[\">]\s*)");
	std::string parentDir = filePath.substr(0, filePath.find_last_of("/\\"));

	while (std::getline(stream, line)) {
		std::smatch match;
		if (std::regex_match(line, match, includeRegex)) {
			std::string includePath = parentDir + "/" + match[1].str();
			std::string includedCode = ParseShaderCode_Internal(includePath, visited);
			parsedCode << "// Begin include: " << includePath << "\n"
				<< includedCode << "\n"
				<< "// End include: " << includePath << "\n";
		}
		else {
			parsedCode << line << "\n";
		}
	}

	return parsedCode.str();
}

std::string ShaderUtilFunctions::ParseShaderCode(const std::string& filePath) {
	std::unordered_set<std::string> visited;
	return ParseShaderCode_Internal(filePath, visited);
}

