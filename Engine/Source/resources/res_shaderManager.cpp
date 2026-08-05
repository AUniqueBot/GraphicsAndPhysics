#include <arch/resources/res_shaderManager.h>


void ShaderManager::Init() {
	
	// - vertex shader ---------------------------------------------------------------------
	GLuint vtxShaderId				{ CreateShader(ShaderConstants::C_ID_VERTEXSHADER) };
	Shader& vertexShader			{ *GetShader(vtxShaderId) };
	std::string vertexShaderSrc { 
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_VERTEXSHADERPATH) 
	};
	vertexShader.SetShaderCode(vertexShaderSrc);
	vertexShader.SetShaderType(ShaderConstants::ShaderType::VERTEX);
	vertexShader.Build();
	AddShader(ShaderConstants::C_ID_VERTEXSHADER, vertexShader);


	// - error frag ------------------------------------------------------------------------
	GLuint unlitErrorFragShaderId	{ CreateShader(ShaderConstants::C_ID_ERRORSHADERPROG) };
	Shader& unlitErrorFragShader	{ *GetShader(unlitErrorFragShaderId) };
	std::string unlitErrorFragSrc{
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_ERRORFRAGSHADERPATH)
	};
	unlitErrorFragShader.SetShaderCode(unlitErrorFragSrc);
	unlitErrorFragShader.SetShaderType(ShaderConstants::ShaderType::FRAG);
	unlitErrorFragShader.Build();
	AddShader(ShaderConstants::C_ID_ERRORFRAGSHADER, unlitErrorFragShader);


	
	// - lambert frag ----------------------------------------------------------------------
	GLuint lambertFragShaderId		{ CreateShader(ShaderConstants::C_ID_LAMBERTFRAGSHADER) };
	std::string lambertFragSrc { 
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_LAMBERTFRAGSHADERPATH) 
	};
	Shader& lambertFragShader		{ *GetShader(lambertFragShaderId) };
	lambertFragShader.SetShaderCode(lambertFragSrc);
	lambertFragShader.SetShaderType(ShaderConstants::ShaderType::FRAG);
	lambertFragShader.Build();
	AddShader(ShaderConstants::C_ID_LAMBERTFRAGSHADER, lambertFragShader);
	
	
	// - phong frag -------------------------------------------------------------------------
	GLuint phongFragShaderId		{ CreateShader(ShaderConstants::C_ID_PHONGSHADERPROG) };
	Shader& phongFragShader			{ *GetShader(phongFragShaderId) };
	std::string phongFragSrc{
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_PHONGFRAGSHADERPATH)
	};
	phongFragShader.SetShaderCode(phongFragSrc);
	phongFragShader.SetShaderType(ShaderConstants::ShaderType::FRAG);
	phongFragShader.Build();
	AddShader(ShaderConstants::C_ID_PHONGFRAGSHADER, phongFragShader);


	// - blinn frag -------------------------------------------------------------------------
	GLuint blinnPhongFragShaderId	{ CreateShader(ShaderConstants::C_ID_BLINNPHONGSHADERPROG) };
	Shader& blinnPhongFragShader	{ *GetShader(blinnPhongFragShaderId) };
	std::string blinnPhongFragSrc{
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_BLINNPHONGFRAGSHADERPATH)
	};
	blinnPhongFragShader.SetShaderCode(blinnPhongFragSrc);
	blinnPhongFragShader.SetShaderType(ShaderConstants::ShaderType::FRAG);
	blinnPhongFragShader.Build();
	AddShader(ShaderConstants::C_ID_BLINNPHONGFRAGSHADER, blinnPhongFragShader);




	// - unlit error shader prg -------------------------------------------------------------
	GLuint unlitErrorShaderId = CreateShaderProgram(ShaderConstants::C_ID_ERRORFRAGSHADER);
	ShaderProgram& unlitErrorShader	{ *GetShaderProgram(unlitErrorShaderId) };
	unlitErrorShader.SetShader(vertexShader, vertexShader.GetShaderType());
	unlitErrorShader.SetShader(unlitErrorFragShader, unlitErrorFragShader.GetShaderType());
	unlitErrorShader.Build();
	

	// - lambert prg ------------------------------------------------------------------------
	GLuint lambertShaderId = CreateShaderProgram(ShaderConstants::C_ID_LAMBERTSHADERPROG);
	ShaderProgram& lambertShader	{ *GetShaderProgram(lambertShaderId) };
	lambertShader.SetShader(vertexShader, vertexShader.GetShaderType());
	lambertShader.SetShader(lambertFragShader, lambertFragShader.GetShaderType());
	lambertShader.Build();
	 
	
	// - phong prg --------------------------------------------------------------------------
	GLuint phongShaderId = CreateShaderProgram(ShaderConstants::C_ID_PHONGSHADERPROG);
	ShaderProgram& phongShader		{ *GetShaderProgram(phongShaderId) };
	phongShader.SetShader(vertexShader, vertexShader.GetShaderType());
	phongShader.SetShader(phongFragShader, phongFragShader.GetShaderType());
	phongShader.Build();


	// - blinn phong prg --------------------------------------------------------------------
	GLuint blinnPhongShaderId = CreateShaderProgram(ShaderConstants::C_ID_BLINNPHONGSHADERPROG);
	ShaderProgram& blinnPhongShader	{ *GetShaderProgram(blinnPhongShaderId) };
	blinnPhongShader.SetShader(vertexShader, vertexShader.GetShaderType());
	blinnPhongShader.SetShader(blinnPhongFragShader, blinnPhongFragShader.GetShaderType());
	blinnPhongShader.Build();

	LOG_INFO("Initialised shader manager");
}

void ShaderManager::Cleanup() {
	for (Shader& shader : m_shaderDatabase) {
		shader.Destroy();
	}
	LOG_INFO("Shader database cleared.");
}



GLuint ShaderManager::AddShader	(
	std::string _identifier, 
	Shader _program
) {
	GLuint handle = GetNewShaderID();
	m_shaderDatabase.Add(std::move(_program), handle);
	m_shaderAliasLookup[_identifier] = handle;
	return handle;
}

void ShaderManager::RemoveShader(std::string _identifier) {
	RemoveShader(m_shaderAliasLookup.at(_identifier));
	m_shaderAliasLookup.erase(_identifier);
}

void ShaderManager::RemoveShader(GLuint _handle) {
	m_shaderDatabase.Remove(_handle);
	m_shaderCounterFree.push(_handle);
}

GLuint ShaderManager::CreateShader(std::string _identifier) {
	GLuint shaderId	{ GetNewShaderID() };
	m_shaderDatabase.Add(std::move(Shader{}), shaderId);
	m_shaderAliasLookup[_identifier] = shaderId;
	return shaderId;
}

SparseSetView<Shader> ShaderManager::GetShader(GLuint _identifier) {
	return m_shaderDatabase[_identifier];
}
SparseSetView<const Shader> ShaderManager::GetShader(GLuint _identifier) const {
	return  m_shaderDatabase.At(_identifier);
}
SparseSetView<Shader> ShaderManager::GetShader(std::string _identifier) {
	return GetShader(m_shaderAliasLookup[_identifier]);
}
SparseSetView<const Shader> ShaderManager::GetShader(std::string _identifier) const {
	return GetShader(m_shaderAliasLookup.at(_identifier));
}



GLuint ShaderManager::AddShaderProgram(std::string _identifier, ShaderProgram _program) {
	GLuint handle = GetNewShaderProgramID();
	m_shaderProgramDatabase.Add(std::move(_program), handle);
	m_shaderProgramAliasLookup[_identifier] = handle;
	return handle;
}

void ShaderManager::RemoveShaderProgram(std::string _identifier) {
	RemoveShaderProgram(m_shaderProgramAliasLookup.at(_identifier));
	m_shaderProgramAliasLookup.erase(_identifier);
}

void ShaderManager::RemoveShaderProgram(GLuint _handle) {
	m_shaderProgramDatabase.Remove(_handle);
}

GLuint ShaderManager::CreateShaderProgram(std::string _identifier) {
	GLuint handle = GetNewShaderProgramID();
	m_shaderProgramDatabase.Add(std::move(ShaderProgram{}), handle);
	m_shaderProgramAliasLookup[_identifier] = handle;
	return handle;
}

SparseSetView<ShaderProgram> ShaderManager::GetShaderProgram(std::string _identifier) {
	return m_shaderProgramDatabase[m_shaderProgramAliasLookup.at(_identifier)];
}

SparseSetView<const ShaderProgram> ShaderManager::GetShaderProgram(std::string _identifier) const {
	return m_shaderProgramDatabase.At(m_shaderProgramAliasLookup.at(_identifier));
}

SparseSetView<ShaderProgram> ShaderManager::GetShaderProgram(GLuint _handle) {
	return m_shaderProgramDatabase[_handle];
}

SparseSetView<const ShaderProgram> ShaderManager::GetShaderProgram(GLuint _handle) const {
	return m_shaderProgramDatabase.At(_handle);
}

SparseSet<GLuint, ShaderProgram>& ShaderManager::GetShaderProgramDatabase() {
	return m_shaderProgramDatabase;
}

const SparseSet<GLuint, ShaderProgram>& ShaderManager::GetShaderProgramDatabase() const {
	return m_shaderProgramDatabase;
}

SparseSet<GLuint, Shader>& ShaderManager::GetShaderDatabase() {
	return m_shaderDatabase;
}

const SparseSet<GLuint, Shader>& ShaderManager::GetShaderDatabase() const {
	return m_shaderDatabase;
}

GLuint ShaderManager::GetNewShaderID() {
	if (m_shaderCounterFree.empty()) return ++m_shaderCounter;
	GLuint retVal{m_shaderCounterFree.front()};
	m_shaderCounterFree.pop();
	return retVal;
}

GLuint ShaderManager::GetNewShaderProgramID() {
	if (m_shaderProgramCounterFree.empty()) return static_cast<GLuint>(++m_shaderProgramCounter);
	GLuint retVal{ m_shaderProgramCounterFree.front() };
	m_shaderProgramCounterFree.pop();
	return retVal;
}

void ShaderManager::ReclaimShaderID(GLuint _id) {
	m_shaderCounterFree.push(_id);
}

void ShaderManager::ReclaimShaderProgramID(GLuint _id) {
	m_shaderProgramCounterFree.push(_id);
}
