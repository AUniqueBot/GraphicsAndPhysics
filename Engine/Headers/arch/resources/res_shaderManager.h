#pragma once
#include <pch.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_shader.h>
#include <arch/resources/res_specializedResourceManager.h>

namespace ShaderConstants {

	// - shader paths ---------------------------------------------------------
	inline constexpr const char C_PATH_VERTEXSHADERPATH[]			{ "./Assets/Shaders/vtx_vertex.vert" };
	inline constexpr const char C_PATH_LAMBERTFRAGSHADERPATH[]		{ "./Assets/Shaders/frag_lambert.frag" };
	inline constexpr const char C_PATH_PHONGFRAGSHADERPATH[]		{ "./Assets/Shaders/frag_phong.frag" };
	inline constexpr const char C_PATH_BLINNPHONGFRAGSHADERPATH[]	{ "./Assets/Shaders/frag_blinnphong.frag" };
	inline constexpr const char C_PATH_ERRORFRAGSHADERPATH[]		{ "./Assets/Shaders/frag_error.frag" };
	// - shader identifiers ---------------------------------------------------
	inline constexpr const char C_ID_VERTEXSHADER[]					{ "C_VERTEX_SHADER" };
	inline constexpr const char C_ID_LAMBERTFRAGSHADER[]			{ "C_LAMBERTFRAG_SHADER" };
	inline constexpr const char C_ID_PHONGFRAGSHADER[]				{ "C_PHONGFRAG_SHADER" };
	inline constexpr const char C_ID_BLINNPHONGFRAGSHADER[]			{ "C_BLINNPHONGFRAG_SHADER" };
	inline constexpr const char C_ID_ERRORFRAGSHADER[]				{ "C_ERRORFRAG_SHADER" };
	// - shader Program -------------------------------------------------------
	inline constexpr const char C_ID_LAMBERTSHADERPROG[]			{ "BRDF_LAMBERT" };
	inline constexpr const char C_ID_PHONGSHADERPROG[]				{ "BRDF_PHONG" };
	inline constexpr const char C_ID_BLINNPHONGSHADERPROG[]			{ "BRDF_BLINNPHONG" };
	inline constexpr const char C_ID_ERRORSHADERPROG[]				{ "ERRORSHADER" };

}

namespace MaterialConstants {
	inline constexpr const char C_ID_LAMBERTSHADERPROG[]			{ "BRDF_LAMBERT" };
	inline constexpr const char C_ID_PHONGSHADERPROG[]				{ "BRDF_PHONG" };
	inline constexpr const char C_ID_BLINNPHONGSHADERPROG[]			{ "BRDF_BLINNPHONG" };
	inline constexpr const char C_ID_ERRORSHADERPROG[]				{ "ERRORSHADER" };
}


class ShaderManager : public SpecializedResourceManager {
public:
	ShaderManager(ResourceManager& _mgr, GPUResourceManager& _gpuMgr) : 
		SpecializedResourceManager(_mgr, _gpuMgr) {}
public:
	void Init() override;
	void Cleanup() override;

	// shader
	ShaderHandle CreateShader(ShaderConstants::ShaderType _type, std::string _code = "");
	std::string GenerateShaderCodeTemplate(ShaderConstants::ShaderType _shaderType);
	ShaderHandle LoadShader(
		const std::filesystem::path& _shaderPath,
		RES_ID _existingId = ResourceConstants::C_RES_INVALID_ID
	);



	void RemoveShader(ShaderHandle _toDelete);
	void RemoveShader(RES_ID _toDelete);
	

	void LoadResource(const Serialization::MetafileData& _meta) override;
public:

	const std::vector<RES_ID>& GetShadersOfType(ShaderConstants::ShaderType _type) const;
private:
	void RegisterFileExtensionToShaderType(const std::string& _extension, ShaderConstants::ShaderType _type);
	ShaderConstants::ShaderType GetShaderType(const std::string& _extension) const;


private:
	std::unordered_map<std::string, ShaderConstants::ShaderType> m_extensionToShaderType;
	std::unordered_map<ShaderConstants::ShaderType, std::vector<RES_ID>> m_shaderIds;
};

// ----------------------------------------------------------------------------------------------


struct RenderShaderProgProps {
	RES_ID vertexShader;
	RES_ID fragShader;
	RES_ID geometryShader				= ResourceConstants::C_RES_INVALID_ID;
	RES_ID tesselationControlShader		= ResourceConstants::C_RES_INVALID_ID;
	RES_ID tesselationEvaluationShader	= ResourceConstants::C_RES_INVALID_ID;
};


class ShaderProgramManager : public SpecializedResourceManager {
public: 
	ShaderProgramManager(ResourceManager& _mgr, GPUResourceManager& _gpuMgr) : 
		SpecializedResourceManager(_mgr, _gpuMgr) {}


	void Init() override;
	void InitShaderPrograms(ShaderManager& _shaderMgr);
	void Cleanup() override;

	//ShaderProgramHandle CreateComputeShaderProgram();
	ShaderProgramHandle CreateRenderShaderProgram(RenderShaderProgProps _props);
	void RemoveShaderProgram(ShaderProgramHandle _toDelete);
	void RemoveShaderProgram(RES_ID _toDelete);


	std::shared_ptr<ShaderProgram> GetShaderProgram(std::string _alias);
	std::shared_ptr<ShaderProgram> GetShaderProgram(RES_ID _id);

private:


};