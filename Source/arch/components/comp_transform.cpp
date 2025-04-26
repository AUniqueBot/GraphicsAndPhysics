
#include <pch.h>
#include <arch/components/comp_transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>


Transform::Transform() {
	std::cout << typeid(Transform).name() << "::Component Added" << std::endl;
}

glm::vec3 Transform::Forward() const
{
	return glm::vec3();
}

glm::mat4 Transform::LocalTransformMtx() const {
	glm::mat4 pos	{ glm::translate(glm::mat4(1.0f), m_pos) };
	glm::mat4 rot	{ glm::mat4_cast(m_rot) };
	glm::mat4 scl	{ glm::scale(glm::mat4(1.0f), m_scl) };

	return pos * rot * scl;
}


