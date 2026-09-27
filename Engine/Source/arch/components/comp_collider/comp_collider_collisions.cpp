#include <arch/components/comp_collider/comp_collider_collisions.h>
#include <glm/gtx/component_wise.hpp>


namespace CollisionFunction {


	CollisionInfo TestDiscreteAABB_Point(const ColliderInfo& _a, const glm::vec3& _b) {

		const AABBShape& collider1 = *reinterpret_cast<const AABBShape*>(_a.collider);
		glm::vec3 apos = _a.position + _a.collider->Offset();
		glm::vec3 bpos = _b;

		glm::vec3 extents = collider1.HalfExtents();
		glm::vec3 delta = bpos - apos;
		glm::vec3 overlap = extents - glm::abs(delta);

		bool collide =
			overlap.x >= 0.0f &&
			overlap.y >= 0.0f &&
			overlap.z >= 0.0f;

		float depth = glm::compMin(overlap);

		glm::vec3 multiplier = glm::sign(delta);

		glm::vec3 normal =
			depth == overlap.x ? glm::vec3(multiplier.x, 0.0f, 0.0f) :
			depth == overlap.y ? glm::vec3(0.0f, multiplier.y, 0.0f) :
			glm::vec3(0.0f, 0.0f, multiplier.z);

		return CollisionInfo{
			.m_normal = normal,
			.m_depth = depth,
			.m_collide = collide
		};
	}


	CollisionInfo TestDiscreteAABB_AABB(
		const ColliderInfo& _a, 
		const ColliderInfo& _b
	) {
		const AABBShape& collider1 = *reinterpret_cast<const AABBShape*>(_a.collider);
		const AABBShape& collider2 = *reinterpret_cast<const AABBShape*>(_b.collider);

		glm::vec3 apos = _a.position + _a.collider->Offset();
		glm::vec3 bpos = _b.position + _b.collider->Offset();

		glm::vec3 extents =
			collider1.HalfExtents() +
			collider2.HalfExtents();

		glm::vec3 delta = bpos - apos;
		glm::vec3 overlap = extents - glm::abs(delta);

		bool collide =
			overlap.x >= 0.0f &&
			overlap.y >= 0.0f &&
			overlap.z >= 0.0f;
	
		float depth = glm::compMin(overlap);
	
		glm::vec3 multiplier = glm::sign(delta);

		glm::vec3 normal =
			depth == overlap.x ? glm::vec3(multiplier.x, 0.0f, 0.0f) :
			depth == overlap.y ? glm::vec3(0.0f, multiplier.y, 0.0f) :
			glm::vec3(0.0f, 0.0f, multiplier.z);

		return CollisionInfo{
			.m_normal = normal,
			.m_depth = depth,
			.m_collide = collide
		};
	}

	CollisionInfo TestDiscreteSphere_Point(const ColliderInfo& _a, const glm::vec3& _b) {
		// radius test
		const SphereShape& collider1 = *reinterpret_cast<const SphereShape*>(_a.collider);

		glm::vec3 apos = _a.position + _a.collider->Offset();
		glm::vec3 bpos = _b;

		glm::vec3 abvec = bpos - apos;

		float totalRadius = collider1.Radius();
		float delta = totalRadius - glm::length(abvec);
		bool collide = delta >= 0;


		return CollisionInfo{
			.m_normal = glm::normalize(abvec),
			.m_depth = std::max(0.f, delta),
			.m_collide = collide
		};
	}

	CollisionInfo TestDiscreteSphere_Sphere(const ColliderInfo& _a, const ColliderInfo& _b) {
		// radius test
		const SphereShape& collider1 = *reinterpret_cast<const SphereShape*>(_a.collider);
		const SphereShape& collider2 = *reinterpret_cast<const SphereShape*>(_b.collider);

		glm::vec3 apos = _a.position + _a.collider->Offset();
		glm::vec3 bpos = _b.position + _b.collider->Offset();

		glm::vec3 abvec = bpos - apos;

		float totalRadius = collider1.Radius() + collider2.Radius();
		float delta = totalRadius - glm::length(abvec);
		bool collide = delta >= 0;


		return CollisionInfo{
			.m_normal = glm::normalize(abvec),
			.m_depth = std::max(0.f, delta),
			.m_collide = collide
		};
	}

	CollisionInfo TestDiscreteAABB_Sphere(const ColliderInfo& _a, const ColliderInfo& _b) {
		
		const AABBShape& collider1 = *reinterpret_cast<const AABBShape*>(_a.collider);
		const SphereShape& collider2 = *reinterpret_cast<const SphereShape*>(_b.collider);

		glm::vec3 apos = _a.position + _a.collider->Offset();
		glm::vec3 bpos = _b.position + _b.collider->Offset();

		glm::vec3 abvec = bpos - apos;

		glm::vec3 halfExtent = collider1.HalfExtents();
		float radius = collider2.Radius();

		bool collides = glm::length(halfExtent) + radius >= glm::length(abvec);
		if (!collides) {
			return CollisionInfo{
				.m_normal = glm::normalize(abvec),
				.m_depth = 0,
				.m_collide = collides,
			};
		}


		// it is not colliding if the length btw a&b is longer than the longest possible length (diagonal

		glm::vec3 multiplier = glm::sign(abvec); 
		glm::vec3 bound = apos + multiplier * halfExtent; 
		glm::vec3 boundToPoint = bpos - bound;



		return CollisionInfo{
			.m_collide = collides
		};
	}




}