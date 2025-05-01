
#include <pch.h>
#include <arch/components/comp_transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

Transform::Transform() {
	std::cout << typeid(Transform).name() << "::Component Added" << std::endl;
}

void Transform::SetRotationEuler(glm::vec3 _rotation) {

}

glm::vec3 Transform::GetRotationEuler() const {	
	glm::vec3 toRet{};
	
	glm::mat4 m{ m_rot };

	float a, b, c;



	b = std::asin(-std::clamp(m[2][0], -1.0f, 1.0f));
	if (std::abs(m[2][0]) < 0.99999f) {
		c = std::atan2(m[1][0], m[0][0]);
		a = std::atan2(m[2][1], m[2][2]);
	}
	else {
		c = std::atan2(-m[0][1], m[1][1]);
		a = 0.0f;
	}

	return toRet;
}



glm::vec3 Transform::Forward() const {
	return glm::vec3(0, 0, 1);
}

glm::vec3 Transform::Up() const {
	return glm::vec3(0, 1, 0);
}

glm::mat4 Transform::LocalTransformMtx() const {
	glm::mat4 pos	{ glm::translate(glm::mat4(1.0f), m_pos) };
	glm::mat4 rot	{ glm::mat4_cast(m_rot) };
	glm::mat4 scl	{ glm::scale(glm::mat4(1.0f), m_scl) };

	return pos * rot * scl;
}




