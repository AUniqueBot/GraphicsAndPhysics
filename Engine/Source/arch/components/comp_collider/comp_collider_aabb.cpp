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
