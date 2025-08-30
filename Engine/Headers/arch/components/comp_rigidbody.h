#pragma once


class RigidBody : public Component {
public:
	RigidBody() {
		
	}


private:

	float m_mass;
	glm::vec3 m_linearVelocity;

};