#pragma once
#include <arch/common/component.h>

class RigidBody : public Component {
public:

	

	const float& Mass() const;
	void Mass(float _value);

	const glm::vec3& LinearVelocity() const;
	void LinearVelocity(const glm::vec3& _value);


	const glm::vec3& LinearAcceleration() const;
	void LinearAcceleration(const glm::vec3& _value);

	const glm::vec3& AngularVelocity() const;
	void AngularVelocity(const glm::vec3& _value);


	const glm::vec3& AngularAcceleration() const;
	void AngularAcceleration(const glm::vec3& _value);



private:

	float m_mass;
	glm::vec3 m_linearVelocity;
	glm::vec3 m_linearAcceleration;

	glm::vec3 m_angularVelocity;
	glm::vec3 m_angularAcceleration;


};