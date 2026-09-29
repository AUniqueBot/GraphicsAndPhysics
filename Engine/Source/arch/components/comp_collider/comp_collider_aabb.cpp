#include <arch/components/comp_collider/comp_collider_aabb.h>

ColliderShapeConstants::Type AABBShape::ColliderType() const {
	return ColliderShapeConstants::Type::AABB;
}

void AABBShape::Dimensions(const glm::vec3& _dimensions) {
	if (_dimensions == m_dimensions) return;
	m_dimensions = _dimensions;
}
const glm::vec3& AABBShape::Dimensions() const {
	return m_dimensions;
}

glm::vec3 AABBShape::HalfExtents() const {
	return m_dimensions * 0.5f;
}


std::vector<PropertyMD::Property>& AABBShape::GetProps() {
	using namespace PropertyMD;
	static std::vector<Property> props = [] {
		std::vector<Property> base = ColliderShape::GetProps();
		base.emplace_back(
			MakeProperty<AABBShape>(
				"Dimensions",
				PropertyType::Float,
				Shape::FixedArray,
				3,
				static_cast<const glm::vec3 & (AABBShape::*)()const>(&AABBShape::Dimensions),
				static_cast<void(AABBShape::*)(const glm::vec3&)>(&AABBShape::Dimensions),
				true
			)
		);
		return base;
		}();

	return props;

}