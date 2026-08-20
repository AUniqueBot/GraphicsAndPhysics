#include <arch/resources/res_shaderManager.h>
#include <arch/resources/res_resourceIdentifier.h>

void ShaderManager::Init() {
	
	// - vertex shader ---------------------------------------------------------------------

	std::string shaderSrc;
	shaderSrc =
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_VERTEXSHADERPATH);
	ShaderHandle vtxShader = CreateShader(ShaderConstants::ShaderType::VERTEX, shaderSrc);
	SetResourceAlias(vtxShader.GetResourceID(), ShaderConstants::C_ID_VERTEXSHADER);

	// - error frag ------------------------------------------------------------------------

	shaderSrc =
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_ERRORFRAGSHADERPATH);
	ShaderHandle errFragShader = CreateShader(ShaderConstants::ShaderType::FRAG, shaderSrc);
	SetResourceAlias(errFragShader.GetResourceID(), ShaderConstants::C_ID_ERRORFRAGSHADER);


	
	// - lambert frag ----------------------------------------------------------------------
	shaderSrc =
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_LAMBERTFRAGSHADERPATH);
	ShaderHandle lambertFragShader = CreateShader(ShaderConstants::ShaderType::FRAG, shaderSrc);
	SetResourceAlias(lambertFragShader.GetResourceID(), ShaderConstants::C_ID_LAMBERTFRAGSHADER);
	
	
	// - phong frag -------------------------------------------------------------------------
	shaderSrc =
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_PHONGFRAGSHADERPATH);
	ShaderHandle phongFragShader = CreateShader(ShaderConstants::ShaderType::FRAG, shaderSrc);
	SetResourceAlias(phongFragShader.GetResourceID(), ShaderConstants::C_ID_PHONGFRAGSHADER);


	// - blinn frag -------------------------------------------------------------------------
	shaderSrc =
		ShaderUtilFunctions::ParseShaderCode(ShaderConstants::C_PATH_BLINNPHONGFRAGSHADERPATH);
	ShaderHandle blinnFragShader = CreateShader(ShaderConstants::ShaderType::FRAG, shaderSrc);
	SetResourceAlias(blinnFragShader.GetResourceID(), ShaderConstants::C_ID_BLINNPHONGFRAGSHADER);
	LOG_INFO("Initialised shader manager");
}

void ShaderManager::Cleanup() {
	for (RES_ID resId : m_resourceIdPool) {
		auto shader = std::static_pointer_cast<Shader>(m_resourceManager.GetResource(resId));
		shader->Destroy();
	}

	LOG_INFO("Shader database cleared.");
}



ShaderHandle ShaderManager::CreateShader(ShaderConstants::ShaderType _type, std::string _code) {
	std::shared_ptr<Shader> res = std::make_shared<Shader>();
	res->SetShaderType(_type);
	res->SetShaderCode(_code);
	res->Build();

	std::string name = ShaderConstants::StringifyShaderType(_type) + " Shader";
	res->Name(name);


	ShaderHandle handle = ShaderHandle(RegisterResource(res));
	if (m_shaderIds.find(_type) == m_shaderIds.end()) m_shaderIds[_type] = {} ;
	m_shaderIds[_type].push_back(handle.GetResourceID());

	return handle;
}

void ShaderManager::RemoveShader(ShaderHandle _toDelete) {
	std::optional<ResourceIdentifier> idr =_toDelete.GetResourceIdentifier();
	if (!idr) return;
	RemoveShader(idr->m_resourceId);
}

void ShaderManager::RemoveShader(RES_ID _toDelete) {
	RemoveAliasForRes(_toDelete);
	Remove(_toDelete);
	// remove
}

const std::vector<RES_ID>& ShaderManager::GetShadersOfType(ShaderConstants::ShaderType _type) const {
	return m_shaderIds.at(_type);
}

// ----------------------------------------------------------------------------------------


void ShaderProgramManager::Init() {

	// get shader resource ids.
	RES_ID vtxShaderRID = m_shaderMgr.GetResIDFromAlias(ShaderConstants::C_ID_VERTEXSHADER);
	RES_ID lambertFragShaderRID = m_shaderMgr.GetResIDFromAlias(ShaderConstants::C_ID_LAMBERTFRAGSHADER);
	RES_ID phongFragShaderRID = m_shaderMgr.GetResIDFromAlias(ShaderConstants::C_ID_PHONGFRAGSHADER);
	RES_ID blinnFragShaderRID = m_shaderMgr.GetResIDFromAlias(ShaderConstants::C_ID_BLINNPHONGFRAGSHADER);
	RES_ID errorFragShaderRID = m_shaderMgr.GetResIDFromAlias(ShaderConstants::C_ID_ERRORFRAGSHADER);

	// generate programs
	ShaderProgramHandle lambert = CreateRenderShaderProgram({ vtxShaderRID, lambertFragShaderRID });
	ShaderProgramHandle phong = CreateRenderShaderProgram({ vtxShaderRID, phongFragShaderRID });
	ShaderProgramHandle blinn = CreateRenderShaderProgram({ vtxShaderRID, blinnFragShaderRID });
	ShaderProgramHandle error = CreateRenderShaderProgram({ vtxShaderRID, errorFragShaderRID });


	// setup aliases.
	SetResourceAlias(lambert.GetResourceID(), ShaderConstants::C_ID_LAMBERTSHADERPROG);
	SetResourceAlias(phong.GetResourceID(), ShaderConstants::C_ID_PHONGSHADERPROG);
	SetResourceAlias(blinn.GetResourceID(), ShaderConstants::C_ID_BLINNPHONGSHADERPROG);
	SetResourceAlias(error.GetResourceID(), ShaderConstants::C_ID_ERRORSHADERPROG);
}

void ShaderProgramManager::Cleanup() {

}

ShaderProgramHandle ShaderProgramManager::CreateRenderShaderProgram(RenderShaderProgProps _props) {
	std::shared_ptr<ShaderProgram> shaderPrg = std::make_shared<ShaderProgram>();
	std::shared_ptr<Shader> vtxShader = std::static_pointer_cast<Shader>(m_resourceManager.GetResource(_props.vertexShader));
	std::shared_ptr<Shader> fragShader = std::static_pointer_cast<Shader>(m_resourceManager.GetResource(_props.fragShader));
	
	shaderPrg->SetShader(*vtxShader);
	shaderPrg->SetShader(*fragShader);

	if (_props.geometryShader != 0) {
		auto shader = std::static_pointer_cast<Shader>(m_resourceManager.GetResource(_props.geometryShader));
		shaderPrg->SetShader(*shader);
	}
	if (_props.tesselationControlShader!= 0) {
		auto shader = std::static_pointer_cast<Shader>(m_resourceManager.GetResource(_props.tesselationControlShader));
		shaderPrg->SetShader(*shader);
	}
	if (_props.tesselationEvaluationShader != 0) {
		auto shader = std::static_pointer_cast<Shader>(m_resourceManager.GetResource(_props.tesselationEvaluationShader));
		shaderPrg->SetShader(*shader);
	}
	shaderPrg->Build();

	ShaderProgramHandle handle(m_resourceManager.AddInternalResource(shaderPrg));
	Add(handle.GetResourceID());
	return handle;
}

void ShaderProgramManager::RemoveShaderProgram(ShaderProgramHandle _toDelete) {
	std::optional<ResourceIdentifier> idr = _toDelete.GetResourceIdentifier();
	if (!idr) return;
	RemoveShaderProgram(idr->m_resourceId);
	
}

void ShaderProgramManager::RemoveShaderProgram(RES_ID _toDelete) {
	RemoveAliasForRes(_toDelete);
	Remove(_toDelete);
}

std::shared_ptr<ShaderProgram> ShaderProgramManager::GetShaderProgram(std::string _alias) {
	return GetShaderProgram(GetResIDFromAlias(_alias));
}

std::shared_ptr<ShaderProgram> ShaderProgramManager::GetShaderProgram(RES_ID _id) {
	return std::static_pointer_cast<ShaderProgram>(m_resourceManager.GetResource(_id));
}
