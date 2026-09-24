#pragma once
#include <arch/common/component.h>

class RigidBody : public Component {
public:

	

	const float& Mass() const;
	void Mass(const float& _value);

	const glm::vec3& LinearVelocity() const;
	void LinearVelocity(const glm::vec3& _value);


	const glm::vec3& LinearAcceleration() const;
	void LinearAcceleration(const glm::vec3& _value);

	const glm::vec3& AngularVelocity() const;
	void AngularVelocity(const glm::vec3& _value);


	const glm::vec3& AngularAcceleration() const;
	void AngularAcceleration(const glm::vec3& _value);

	const bool& Gravity() const;
	void Gravity(const bool& _value);

private:

	float m_mass							{ 1.0f };
	glm::vec3 m_linearVelocity				{ 0.f, 0.f, 0.f };
	glm::vec3 m_linearAcceleration			{ 0.f, 0.f, 0.f };

	glm::vec3 m_angularVelocity				{ 0.f, 0.f, 0.f };
	glm::vec3 m_angularAcceleration			{ 0.f, 0.f, 0.f };


	bool m_gravity							{ false };

	INSPECTABLE_DECLAREPROPS(RigidBody);
};