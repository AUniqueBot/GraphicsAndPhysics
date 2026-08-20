#pragma once
#include <pch.h>
#include <rapidjson/document.h>
#include <arch/resources/res_material.h>
#include <arch/resources/res_material_presets/res_materiallist.h>
#include <arch/resources/res_specializedResourceManager.h>



class MaterialManager : public SpecializedResourceManager {

public:
	MaterialManager(ResourceManager& _mgr, GPUResourceManager& _gpuMgr) : SpecializedResourceManager(_mgr, _gpuMgr) {}
	void Init() override;
	

	//! @brief returns the shader id of the current material.
	GLuint ResolveMaterial(std::string _materialID) const; // for use in the Rendering System.
	// this would be incorrect; and should be in the shader manager instead.
	



	// - factory ---------------------------------------
	MaterialHandle						CreateGenericMaterial();
	MaterialHandle						CreateUnlitMaterial();
	LambertMaterialHandle				CreateLambertMaterial();
	PhongMaterialHandle					CreatePhongMaterial();
	BlinnPhongMaterialHandle			CreateBlinnMaterial();
	MaterialHandle						CreateGGXMaterial();

	// - loading from serialization --------------------
	MaterialHandle LoadMaterial(
		const rapidjson::Value& _materialData, 
		RES_ID _existingId = ResourceConstants::C_RES_INVALID_ID
	);
	
	std::shared_ptr<LambertMaterial> LoadLambertMaterial(const rapidjson::Value& _materialData);
	std::shared_ptr<PhongMaterial> LoadPhongMaterial(const rapidjson::Value& _materialData);
	std::shared_ptr<BlinnPhongMaterial> LoadBlinnMaterial(const rapidjson::Value& _materialData);
	std::shared_ptr<Material> LoadGGXMaterial(const rapidjson::Value& _materialData);

	void LoadResource(const MetafileData& _data) override;

private:


};