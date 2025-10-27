#pragma once


class RigidBody : public Component {
public:
	RigidBody() {
		std::cout << __FUNCSIG__ << std::endl;
	}


private:

	float m_mass;
	glm::vec3 m_linearVelocity;

};