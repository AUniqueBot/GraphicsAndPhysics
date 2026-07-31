#include <arch/resources/res_material_presets/res_material_blinnphong.h>

Materials::ShadingModel BlinnPhongMaterial::GetShadingModel() const {
    return Materials::ShadingModel::BLINN_PHONG;
}

std::vector<PropertyMD::Property>& BlinnPhongMaterial::GetProps() {
    using namespace PropertyMD;
    static std::vector<Property> props{
        PropertyMD::MakeProperty<BlinnPhongMaterial>(
            "Color", PropertyType::Color, PropertyMD::Shape::FixedArray, 4,
            static_cast<const glm::vec4 & (BlinnPhongMaterial::*)() const>(&BlinnPhongMaterial::Color),
            static_cast<void(BlinnPhongMaterial::*)(const glm::vec4&)>(&BlinnPhongMaterial::Color)
        ),
        PropertyMD::MakeProperty<PhongMaterial>(
            "Specular", PropertyType::Color, PropertyMD::Shape::FixedArray, 4,
            static_cast<const glm::vec4 & (PhongMaterial::*)() const>(&BlinnPhongMaterial::Specular),
            static_cast<void(PhongMaterial::*)(const glm::vec4&)>(&BlinnPhongMaterial::Specular)
        ),
        PropertyMD::MakeProperty<BlinnPhongMaterial>(
            "Gloss", PropertyType::Float, PropertyMD::Shape::Scalar, 1,
            static_cast<const float& (BlinnPhongMaterial::*)() const>(&BlinnPhongMaterial::Gloss),
            static_cast<void(BlinnPhongMaterial::*)(float)>(&BlinnPhongMaterial::Gloss),
            true
        ),
    };
    return props;
}