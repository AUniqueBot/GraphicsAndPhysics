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