#include <arch/resources/res_material_presets/res_material_blinnphong.h>

Materials::ShadingModel BlinnPhongMaterialRes::GetShadingModel() const {
    return Materials::ShadingModel::BLINN_PHONG;
}

std::vector<PropertyMD::Property>& BlinnPhongMaterialRes::GetProps() {
    using namespace PropertyMD;
    static std::vector<Property> props = [] {
        std::vector<Property> props = PhongMaterialRes::GetProps();
        return props;
    }();
    return props;
}