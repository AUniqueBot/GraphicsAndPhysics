#include <arch/components/comp_collider/comp_collider_sphere.h>


ColliderShapeConstants::Type SphereShape::ColliderType() const {
	return ColliderShapeConstants::Type::Sphere;
}

const float& SphereShape::Radius() const {
	return m_radius;
}
void SphereShape::Radius(const float& _radius) {
	if (m_radius == _radius) return;
	m_radius = _radius;
}

std::vector<PropertyMD::Property>& SphereShape::GetProps() {
	using namespace PropertyMD;
	static std::vector<Property> props = []{
		std::vector<Property> base = ColliderShape::GetProps();
		base.emplace_back(
			MakeProperty<SphereShape>(
				"Radius",
				PropertyType::Float,
				Shape::Scalar,
				1,
				static_cast<const float& (SphereShape::*)()const>(&SphereShape::Radius),
				static_cast<void(SphereShape::*)(const float&)>(&SphereShape::Radius),
				true
			)
		);
		return base;
	}();

	return props;

}