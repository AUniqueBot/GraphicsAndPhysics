#include <arch/resources/res_material_presets/res_material_blinnphong.h>

Materials::ShadingModel BlinnPhongMaterialRes::GetShadingModel() const {
    return Materials::ShadingModel::BLINN_PHONG;
}

std::vector<PropertyMD::Property>& BlinnPhongMaterialRes::GetProps() {
    return PhongMaterialRes::GetProps();
}