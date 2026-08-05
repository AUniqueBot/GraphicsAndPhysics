#pragma once
#include <pch.h>
#include <arch/resources/res_material.h>
#include <arch/resources/res_material_presets/res_materiallist.h>
#include <arch/resources/res_specializedResourceManager.h>



class MaterialManager : public SpecializedResourceManager {

public:
	MaterialManager(ResourceManager& _mgr) : SpecializedResourceManager(_mgr) {}
	void Init() override;
	

	//! @brief returns the shader id of the current material.
	GLuint ResolveMaterial(std::string _materialID) const; // for use in the Rendering System.
	// this would be incorrect; and should be in the shader manager instead.
	


	// - factory ---------------------------------------
	
	std::shared_ptr<Material>			CreateGenericMaterial();
	std::shared_ptr<Material>			CreateUnlitMaterial();
	std::shared_ptr<LambertMaterial>	CreateLambertMaterial();
	std::shared_ptr<PhongMaterial>		CreatePhongMaterial();
	std::shared_ptr<BlinnPhongMaterial>	CreateBlinnMaterial();
	std::shared_ptr<Material>			CreateGGXMaterial();



private:
	// storage of all materials
	SparseSet<RES_ID, std::shared_ptr<Material>> m_materialStorage;

};