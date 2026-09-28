#pragma once
#include <pch.h>
#include <arch/common/inspectable.h>

namespace ColliderShapeConstants {
	enum class Type : uint32_t {
		Sphere,
		Capsule,
		AABB,
		OBB,
		Mesh
	};
};


class ColliderShape : public Inspectable {
public:
	virtual ColliderShapeConstants::Type ColliderType() const = 0;
public:
	const glm::vec3& Offset() const;
	void Offset(const glm::vec3& _offset);
	
private:
	glm::vec3 m_offset{};

	INSPECTABLE_DECLAREPROPS(ColliderShape);
};

