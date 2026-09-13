#pragma once
#include <pch.h>
#include <arch/resources/res_material_presets/res_material_phong.h>


#include <optional>


class BlinnPhongMaterialRes : public PhongMaterialRes {

public:
	Materials::ShadingModel GetShadingModel() const override;
	RESOURCE_TYPENAMEOVERRIDE(Blinn);
	INSPECTABLE_DECLAREPROPS(BlinnPhongMaterialRes);

};



using BlinnPhongMaterialHandle = ResourceHandleDerivedT<MaterialRes, BlinnPhongMaterialRes>;