#include <arch/components/comp_rigidbody.h>



const float& RigidBody::Mass() const {
	return m_mass;
}
void RigidBody::Mass(float _value) {
	if (m_mass == _value) return;
	m_mass = _value;

}

const glm::vec3& RigidBody::LinearVelocity() const {
	return m_linearVelocity;
}

void RigidBody::LinearVelocity(const glm::vec3& _value) {
	if (m_linearVelocity == _value) return;
	m_linearVelocity = _value;
}

const glm::vec3& RigidBody::LinearAcceleration() const {
	return m_linearAcceleration;
}

void RigidBody::LinearAcceleration(const glm::vec3& _value) {
	if (m_linearAcceleration == _value) return;
	m_linearAcceleration = _value;
}

const glm::vec3& RigidBody::AngularVelocity() const {
	return m_angularVelocity;
	// TODO: insert return statement here
}

void RigidBody::AngularVelocity(const glm::vec3& _value) {
	if (m_angularVelocity == _value) return;
	m_angularVelocity = _value;
}

const glm::vec3& RigidBody::AngularAcceleration() const {
	return m_angularAcceleration;
}

void RigidBody::AngularAcceleration(const glm::vec3& _value) {
	if (m_angularAcceleration == _value) return;
	m_angularAcceleration = _value;
}




