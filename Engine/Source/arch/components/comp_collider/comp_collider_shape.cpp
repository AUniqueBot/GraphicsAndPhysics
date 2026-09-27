#include <arch/components/comp_collider/comp_collider_shape.h>


const glm::vec3& ColliderShape::Offset() const {
	return m_offset;
}
void ColliderShape::Offset(const glm::vec3& _offset) {
	if (_offset == m_offset) return;
	m_offset = _offset;	
}