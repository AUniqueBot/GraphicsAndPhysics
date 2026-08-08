#pragma once
#include <pch.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_shader.h>
#include <arch/resources/res_specializedResourceManager.h>

namespace ShaderConstants {

	// - shader paths ---------------------------------------------------------
	inline constexpr const char* C_PATH_VERTEXSHADERPATH			{ "./Assets/Shaders/vtx_vertex.vert" };
	inline constexpr const char* C_PATH_LAMBERTFRAGSHADERPATH		{ "./Assets/Shaders/frag_lambert.frag" };
	inline constexpr const char* C_PATH_PHONGFRAGSHADERPATH			{ "./Assets/Shaders/frag_phong.frag" };
	inline constexpr const char* C_PATH_BLINNPHONGFRAGSHADERPATH	{ "./Assets/Shaders/frag_blinnphong.frag" };
	inline constexpr const char* C_PATH_ERRORFRAGSHADERPATH			{ "./Assets/Shaders/frag_error.frag" };
	// - shader identifiers ---------------------------------------------------
	inline constexpr const char* C_ID_VERTEXSHADER					{ "C_VERTEX_SHADER" };
	inline constexpr const char* C_ID_LAMBERTFRAGSHADER				{ "C_LAMBERTFRAG_SHADER" };
	inline constexpr const char* C_ID_PHONGFRAGSHADER				{ "C_PHONGFRAG_SHADER" };
	inline constexpr const char* C_ID_BLINNPHONGFRAGSHADER			{ "C_BLINNPHONGFRAG_SHADER" };
	inline constexpr const char* C_ID_ERRORFRAGSHADER				{ "C_ERRORFRAG_SHADER" };
	// - shader Program -------------------------------------------------------
	inline constexpr const char* C_ID_LAMBERTSHADERPROG				{ "BRDF_LAMBERT" };
	inline constexpr const char* C_ID_PHONGSHADERPROG				{ "BRDF_PHONG" };
	inline constexpr const char* C_ID_BLINNPHONGSHADERPROG			{ "BRDF_BLINNPHONG" };
	inline constexpr const char* C_ID_ERRORSHADERPROG				{ "ERRORSHADER" };

}

namespace MaterialConstants {
	inline constexpr const char* C_ID_LAMBERTSHADERPROG				{ "BRDF_LAMBERT" };
	inline constexpr const char* C_ID_PHONGSHADERPROG				{ "BRDF_PHONG" };
	inline constexpr const char* C_ID_BLINNPHONGSHADERPROG			{ "BRDF_BLINNPHONG" };
	inline constexpr const char* C_ID_ERRORSHADERPROG				{ "ERRORSHADER" };
}


class ShaderManager : public SpecializedResourceManager {
public:
	ShaderManager(ResourceManager& _mgr) : SpecializedResourceManager(_mgr) {}
public:
	void Init() override;
	void Cleanup() override;


	// shader
	ShaderHandle CreateShader(ShaderConstants::ShaderType _type, std::string _code = "");
	void RemoveShader(ShaderHandle _toDelete);
	void RemoveShader(RES_ID _toDelete);
	
public:
	const std::vector<RES_ID>& GetShadersOfType(ShaderConstants::ShaderType _type) const;
private:
	std::unordered_map<ShaderConstants::ShaderType, std::vector<RES_ID>> m_shaderIds;
};

// ----------------------------------------------------------------------------------------------


struct RenderShaderProgProps {
	RES_ID vertexShader;
	RES_ID fragShader;
	RES_ID geometryShader				= BaseResource::C_RES_ID_INVALID;
	RES_ID tesselationControlShader		= BaseResource::C_RES_ID_INVALID;
	RES_ID tesselationEvaluationShader	= BaseResource::C_RES_ID_INVALID;
};


class ShaderProgramManager : public SpecializedResourceManager {
public: 
	ShaderProgramManager(ResourceManager& _mgr, ShaderManager& _shaderMgr) : 
		SpecializedResourceManager(_mgr), m_shaderMgr(_shaderMgr) {}


	void Init() override;
	void Cleanup() override;

	//ShaderProgramHandle CreateComputeShaderProgram();
	ShaderProgramHandle CreateRenderShaderProgram(RenderShaderProgProps _props);
	void RemoveShaderProgram(ShaderProgramHandle _toDelete);
	void RemoveShaderProgram(RES_ID _toDelete);


	std::shared_ptr<ShaderProgram> GetShaderProgram(std::string _alias);
	std::shared_ptr<ShaderProgram> GetShaderProgram(RES_ID _id);

private:
	ShaderManager& m_shaderMgr;

};