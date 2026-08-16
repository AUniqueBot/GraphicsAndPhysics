#include <arch/resources/res_material_presets/res_material_blinnphong.h>

Materials::ShadingModel BlinnPhongMaterial::GetShadingModel() const {
    return Materials::ShadingModel::BLINN_PHONG;
}

std::vector<PropertyMD::Property>& BlinnPhongMaterial::GetProps() {
    return PhongMaterial::GetProps();
}