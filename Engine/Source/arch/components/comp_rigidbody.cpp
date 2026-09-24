#include <arch/components/comp_rigidbody.h>



const float& RigidBody::Mass() const {
	return m_mass;
}
void RigidBody::Mass(const float& _value) {
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

const bool& RigidBody::Gravity() const {
	return m_gravity;
}

void RigidBody::Gravity(const bool& _value) {
	if (_value == m_gravity) return;
	m_gravity = _value;
}


std::vector<PropertyMD::Property>& RigidBody::GetProps() {
	static std::vector<PropertyMD::Property> props{
		PropertyMD::MakeProperty<RigidBody>(
			"Mass", 
			PropertyMD::PropertyType::Float,
			PropertyMD::Shape::Scalar,
			1,
			static_cast<const float&(RigidBody::*)() const>(&RigidBody::Mass),
			static_cast<void (RigidBody::*)(const float&)>(&RigidBody::Mass),
			true
		),
		PropertyMD::MakeProperty<RigidBody>(
			"Gravity",
			PropertyMD::PropertyType::Boolean,
			PropertyMD::Shape::Scalar,
			1,
			static_cast<const bool& (RigidBody::*)() const>(&RigidBody::Gravity),
			static_cast<void (RigidBody::*)(const bool&)>(&RigidBody::Gravity),
			true
		),
		
		// read only	
		PropertyMD::MakeProperty<RigidBody>(
			"Acceleration",
			PropertyMD::PropertyType::Float,
			PropertyMD::Shape::FixedArray,
			3,
			static_cast<const glm::vec3& (RigidBody::*)() const>(&LinearAcceleration),
			static_cast<void (RigidBody::*)(const glm::vec3&)>(&LinearAcceleration),
			true
		),
		PropertyMD::MakeProperty<RigidBody>(
			"Velocity",
			PropertyMD::PropertyType::Float,
			PropertyMD::Shape::FixedArray,
			3,
			static_cast<const glm::vec3 & (RigidBody::*)() const>(&LinearVelocity),
			static_cast<void (RigidBody::*)(const glm::vec3&)>(&LinearVelocity),
			true
		),

	};

	return props;
}


