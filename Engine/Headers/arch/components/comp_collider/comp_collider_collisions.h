#pragma once
#include <pch.h>
#include <arch/components/comp_collider/comp_collider_aabb.h>
#include <arch/components/comp_collider/comp_collider_sphere.h>

struct ColliderInfo {
	const ColliderShape* collider;
	glm::vec3 position;	
	glm::quat rotation;
};

// tests a->b
// info is based on a; normal is surface normal of A of collision.
struct CollisionInfo {
	glm::vec3 m_normal;
	float m_depth;
	bool m_collide;
};

using CollisionTestFunction = std::function<CollisionInfo(const ColliderInfo&, const ColliderInfo&)>;

using CollisionTestFunctionDB = std::map<
	std::pair<ColliderShapeConstants::Type, ColliderShapeConstants::Type>,
	CollisionTestFunction
>;

namespace CollisionFunction {

	CollisionInfo TestDiscreteAABB_Point(const ColliderInfo& _a, const glm::vec3& _b);
	CollisionInfo TestDiscreteAABB_AABB(const ColliderInfo& _a, const ColliderInfo& _b);

	CollisionInfo TestDiscreteSphere_Point(const ColliderInfo& _a, const glm::vec3& _b);
	CollisionInfo TestDiscreteSphere_Sphere(const ColliderInfo& _a, const ColliderInfo& _b);



	
	CollisionInfo TestDiscreteAABB_Sphere(const ColliderInfo& _a, const ColliderInfo& _b);


	CollisionInfo TestDiscreteCapsule_Capsule(const ColliderInfo& _a, const ColliderInfo& _b);
	


	/*
		dictionary
		std::unordered_map<std::pair<CollisionShapeConstants::Type, CollisionShapeConstants::Type>, CollisionTestFunction> functionDict

		
	
	*/
};