#pragma once
#include <pch.h>
#include <arch/components/comp_collider/comp_collider_shape.h>




class AABBShape : public ColliderShape {

public:
	AABBShape(glm::fvec3 _dimensions = glm::fvec3(1.f, 1.f, 1.f)) : m_dimensions(_dimensions) {}
	ColliderShapeConstants::Type ColliderType() const override;
public:
	void Dimensions(const glm::vec3& _dimensions);
	const glm::vec3& Dimensions() const;

	glm::vec3 HalfExtents() const;

private:
	glm::fvec3 m_dimensions;

public:
	INSPECTABLE_DECLAREPROPS(AABBShape);
};

