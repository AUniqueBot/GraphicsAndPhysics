#include <arch/components/comp_collider/comp_collider_shape.h>


const glm::vec3& ColliderShape::Offset() const {
	return m_offset;
}
void ColliderShape::Offset(const glm::vec3& _offset) {
	if (_offset == m_offset) return;
	m_offset = _offset;	
}

std::vector<PropertyMD::Property>& ColliderShape::GetProps() {
	using namespace PropertyMD;
	static std::vector<Property> props{
		MakeProperty<ColliderShape>(
			"Offset",
			PropertyType::Float,
			Shape::FixedArray,
			3,
			static_cast<const glm::vec3&(ColliderShape::*)() const>(&ColliderShape::Offset),
			static_cast<void(ColliderShape::*)(const glm::vec3&)>(&ColliderShape::Offset),
			true
		)
	};

	return props;
}