#include <arch/components/comp_collider.h>


void Collider::Init(){

}
void Collider::Destroy() {

}


const ColliderConstants::MotionType& Collider::MotionType() const {
	return m_motionType;
}
void Collider::MotionType(
	const ColliderConstants::MotionType& _motionType
) {
	if (_motionType == m_motionType) return;
	m_motionType = _motionType;
}

void Collider::AddCollider(std::shared_ptr<ColliderShape> _shape) {
	m_collisionShapes.push_back(_shape);
}

void Collider::RemoveCollider(int _idx) {

	// 
	std::rotate(
		m_collisionShapes.begin() + _idx,
		m_collisionShapes.begin() + _idx + 1,
		m_collisionShapes.end());
	m_collisionShapes.pop_back();
}

ColliderShape* Collider::GetCollider(int _idx) {
	return m_collisionShapes[_idx].get();
}

const ColliderShape* Collider::GetCollider(int _idx) const {
	return m_collisionShapes[_idx].get();
}

std::vector<std::shared_ptr<ColliderShape>>& Collider::GetColliderList() {
	return m_collisionShapes;
}

const std::vector<std::shared_ptr<ColliderShape>>& Collider::GetColliderList() const {
	return m_collisionShapes;
}


std::vector<PropertyMD::Property>& Collider::GetProps() {
	using namespace PropertyMD;
	using MT = ColliderConstants::MotionType;
	using ColliderList = std::vector<std::shared_ptr<ColliderShape>>;


	static std::vector<Property> props{
		MakeEnumProperty<Collider, MT>(
			"Motion Type",
			static_cast<const MT & (Collider::*)()const>(&Collider::MotionType),
			static_cast<void(Collider::*)(const MT&)>(&Collider::MotionType),
			{
				{ "Static", static_cast<int>(MT::Static) },
				{ "Dynamic", static_cast<int>(MT::Dynamic) },
				{ "Animated", static_cast<int>(MT::Animated) }
			}
		),
		MakeListProperty<Collider, std::shared_ptr<ColliderShape>>(
			"Collider Shapes",
			PropertyType::Pointer,
			static_cast<ColliderList&(Collider::*)()>(&Collider::GetColliderList),
			&Collider::AddCollider,
			&Collider::RemoveCollider
		)
	};
	
	return props;
}