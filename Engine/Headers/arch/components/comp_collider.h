#pragma once
#include <arch/common/component.h>
#include <arch/components/comp_collider/comp_collider_sphere.h>


namespace ColliderConstants {
	enum class MotionType : int32_t {
		Static,		
		Dynamic,
		Animated
	};
};

class Collider : public Component {
public:
	void Init();
	void Destroy();
	
	const ColliderConstants::MotionType& MotionType() const;
	void MotionType(const ColliderConstants::MotionType& _motionType);

	void AddCollider(std::shared_ptr<ColliderShape> _shape);
	void RemoveCollider(int _idx);
	ColliderShape* GetCollider(int _idx);
	const ColliderShape* GetCollider(int _idx) const;

	std::vector<std::shared_ptr<ColliderShape>>& GetColliderList();
	const std::vector<std::shared_ptr<ColliderShape>>& GetColliderList() const;
	

private:
	ColliderConstants::MotionType m_motionType		{ ColliderConstants::MotionType::Static };

	
	std::vector<std::shared_ptr<ColliderShape>> m_collisionShapes;
	
public:
	INSPECTABLE_DECLAREPROPS(Collider);

};