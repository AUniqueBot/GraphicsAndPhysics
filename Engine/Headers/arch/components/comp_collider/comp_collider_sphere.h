#pragma once
#include <pch.h>
#include <arch/components/comp_collider/comp_collider_shape.h>




class SphereShape : public ColliderShape {
public:
	SphereShape(float _radius = 0.5f) : m_radius(_radius) {}
	ColliderShapeConstants::Type ColliderType() const override;
public:
	const float& Radius() const;
	void Radius(const float& _radius);
private:
	float m_radius;

	INSPECTABLE_DECLAREPROPS(SphereShape);
};