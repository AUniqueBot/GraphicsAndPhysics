#include <arch/resources/res_materialManager.h>
#include <arch/resources/res_shaderManager.h>

void MaterialManager::Init() {

}

GLuint MaterialManager::ResolveMaterial(std::string _materialID) const {

	return 0;
	
}

MaterialHandle MaterialManager::CreateGenericMaterial() {
	std::shared_ptr<Material> matPtr { std::make_shared<Material>() };
	MaterialHandle mat(RegisterResource(matPtr));
	return mat;
}

MaterialHandle MaterialManager::CreateUnlitMaterial() {
	return MaterialHandle(std::nullopt);
}

LambertMaterialHandle MaterialManager::CreateLambertMaterial() {
	std::shared_ptr<LambertMaterial> mat { std::make_shared<LambertMaterial>() };
	LambertMaterialHandle res (RegisterResource(mat));
	return res;
}

PhongMaterialHandle MaterialManager::CreatePhongMaterial() {
	std::shared_ptr<PhongMaterial> mat{ std::make_shared<PhongMaterial>() };
	PhongMaterialHandle res(RegisterResource(mat));
	return res;
}

BlinnPhongMaterialHandle MaterialManager::CreateBlinnMaterial() {
	std::shared_ptr<BlinnPhongMaterial> mat{ std::make_shared<BlinnPhongMaterial>() };
	BlinnPhongMaterialHandle handle(m_resourceManager.AddInternalResource(mat));
	return handle;
}

MaterialHandle MaterialManager::CreateGGXMaterial() {
	return MaterialHandle(std::nullopt);
}

std::shared_ptr<Material> MaterialManager::LoadMaterial(const rapidjson::Value& _materialData) {
	/*
	example of a material file.
	{
		"name": "TestLambert",
		"shader_id": "C_ID_LAMBERTSHADERPROG",
		"properties" : [
			{
				"albedo": {
					"type" : "color",
					"value" : [0.5, 0.5, 0.5, 1.0] 
				}
			}
		]
    
	}
	*/

	// check the shader_id.

	const rapidjson::Value& shader = _materialData["shader_id"];
	std::shared_ptr<Material> mat;
	if (shader.IsString()) {
		// shader uses an alias (human read), load that.
		std::string alias = shader.GetString();
		const rapidjson::Value& props = _materialData["properties"];
		if (alias == ShaderConstants::C_ID_LAMBERTSHADERPROG) {
			return LoadLambertMaterial(props);
		}
		else if (alias == ShaderConstants::C_ID_LAMBERTSHADERPROG) {
			return LoadPhongMaterial(props);
		}
		else if (alias == ShaderConstants::C_ID_BLINNPHONGSHADERPROG) {
			return LoadBlinnMaterial(props);
		}

	}

	else {
		// get resource id.
		RES_ID id = shader.GetInt64();
		// 
	}



	return std::shared_ptr<Material>();
}


std::shared_ptr<LambertMaterial> MaterialManager::LoadLambertMaterial(const rapidjson::Value& _materialData) {
	return std::shared_ptr<LambertMaterial>();
}

std::shared_ptr<PhongMaterial> MaterialManager::LoadPhongMaterial(const rapidjson::Value& _materialData) {
	return std::shared_ptr<PhongMaterial>();
}

std::shared_ptr<BlinnPhongMaterial> MaterialManager::LoadBlinnMaterial(const rapidjson::Value& _materialData) {
	return std::shared_ptr<BlinnPhongMaterial>();
}

std::shared_ptr<Material> MaterialManager::LoadGGXMaterial(const rapidjson::Value& _materialData) {
	return std::shared_ptr<Material>();
}

