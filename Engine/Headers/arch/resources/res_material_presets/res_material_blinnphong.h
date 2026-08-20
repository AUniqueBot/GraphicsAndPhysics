#pragma once
#include <pch.h>
#include <arch/resources/res_material_presets/res_material_phong.h>


#include <optional>


class BlinnPhongMaterial : public PhongMaterial {

public:
	Materials::ShadingModel GetShadingModel() const override;

	INSPECTABLE_DECLAREPROPS(BlinnPhongMaterial);

};



using BlinnPhongMaterialHandle = ResourceHandleDerivedT<Material, BlinnPhongMaterial>;