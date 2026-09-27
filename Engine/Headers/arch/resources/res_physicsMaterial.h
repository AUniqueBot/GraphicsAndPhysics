#pragma once
#include <arch/resources/res_resource.h>

class PhysicsMaterialRes : public Resource<PhysicsMaterialRes> {

public:
	const float& Restitution() const;
	void Restitution(const float& _value);
	
	const float& StaticFriction() const;
	void StaticFriction(const float& _value);

	const float& DynamicFriction() const;
	void DynamicFriction(const float& _value);

private:
	float m_restitution			{};
	float m_staticFriction		{};
	float m_dynamicFriction		{};


};