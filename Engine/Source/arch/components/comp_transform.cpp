
#include <pch.h>
#include <arch/components/comp_transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

Transform::Transform() {
	LOG_INFO("Adding Transform Component");
}

void Transform::Init() {
	LOG_INFO("Init Transform Component");
	m_scl = glm::vec3(1.f);
}

void Transform::End() {
	LOG_INFO("End Transform Component");
}

void Transform::RotationEuler(glm::vec3 _rotation) {
	// convert to quat.

	_rotation = glm::radians(_rotation);

	glm::quat qx = glm::angleAxis(_rotation.x, glm::vec3(1, 0, 0));
	glm::quat qy = glm::angleAxis(_rotation.y, glm::vec3(0, 1, 0));
	glm::quat qz = glm::angleAxis(_rotation.z, glm::vec3(0, 0, 1));

	switch (m_rotOrder) {
	case ROTATION_ORDER::XYZ:
		m_rot = qz * qy * qx;
		break;
	case ROTATION_ORDER::XZY:
		m_rot = qy * qz * qx;
		break;
	case ROTATION_ORDER::YXZ:
		m_rot = qz * qx * qy;
		break;
	case ROTATION_ORDER::YZX:
		m_rot = qx * qz * qy;
		break;
	case ROTATION_ORDER::ZXY:
		m_rot = qy * qx * qz;
		break;
	case ROTATION_ORDER::ZYX:
		m_rot = qx * qy * qz;
		break;
	}
}

glm::vec3 Transform::RotationEuler() const {
	return
		m_rotOrder == ROTATION_ORDER::XYZ ? QuatToXYZ() :
		m_rotOrder == ROTATION_ORDER::XYZ ? QuatToXYZ() :
		m_rotOrder == ROTATION_ORDER::XYZ ? QuatToXYZ() :
		m_rotOrder == ROTATION_ORDER::XYZ ? QuatToXYZ() :
		m_rotOrder == ROTATION_ORDER::XYZ ? QuatToXYZ() :
		m_rotOrder == ROTATION_ORDER::XYZ ? QuatToXYZ() :
		glm::vec3();
}


glm::vec3 Transform::Forward() const {
	// forward vectors is always assumed to be the z axis.
	return m_rot * glm::vec3(0, 0, 1);
}

void Transform::Forward(glm::vec3 _newForward) {
	_newForward = glm::normalize(_newForward);
	glm::vec3 right = glm::normalize(glm::cross(Up(), _newForward));
	glm::vec3 trueUp = glm::cross(_newForward, right);

	glm::mat3 rotationMatrix(right, trueUp, _newForward); // column-major by default
	m_rot = glm::quat_cast(rotationMatrix);
}

glm::vec3 Transform::Up() const {
	// up vector is assumed to be the y axis.
	return m_rot * glm::vec3(0, 1, 0);
}

glm::mat4 Transform::LocalTransformMtx() const {
	glm::mat4 pos	{ glm::translate(glm::mat4(1.0f), m_pos) };
	glm::mat4 rot	{ glm::mat4_cast(m_rot) };
	glm::mat4 scl	{ glm::scale(glm::mat4(1.0f), m_scl) };

	return pos * rot * scl;
}




glm::vec3 Transform::QuatToXYZ() const  {
	return glm::vec3(
		atan2(2.0f * (m_rot.w * m_rot.x - m_rot.y * m_rot.z), -2.0f * (m_rot.x * m_rot.x + m_rot.y * m_rot.y) + 1.0f),
		asin(2.0f * (m_rot.w * m_rot.y + m_rot.x * m_rot.z)),
		atan2(2.0f * (m_rot.w * m_rot.z - m_rot.x * m_rot.y), -2.0f * (m_rot.y * m_rot.y + m_rot.z * m_rot.z) + 1.0f)
	);
}
glm::vec3 Transform::QuatToXZY() const  {
	return glm::vec3(
		atan2(2.0f * (m_rot.w * m_rot.x + m_rot.y * m_rot.z), -2.0f * (m_rot.x * m_rot.x + m_rot.z * m_rot.z) + 1.0f),
		atan2(2.0f * (m_rot.w * m_rot.y - m_rot.x * m_rot.z), -2.0f * (m_rot.y * m_rot.y + m_rot.z * m_rot.z) + 1.0f),
		asin(2.0f * (m_rot.w * m_rot.z - m_rot.x * m_rot.y))
	);
}
glm::vec3 Transform::QuatToYXZ() const  {
	return glm::vec3(
		asin(2.0f * (m_rot.w * m_rot.x - m_rot.y * m_rot.z)),
		atan2(2.0f * (m_rot.w * m_rot.y + m_rot.x * m_rot.z), -2.0f * (m_rot.x * m_rot.x + m_rot.y * m_rot.y) + 1.0f),
		atan2(2.0f * (m_rot.w * m_rot.z + m_rot.x * m_rot.y), -2.0f * (m_rot.x * m_rot.x + m_rot.z * m_rot.z) + 1.0f)
	);
}
glm::vec3 Transform::QuatToYZX() const  {
	return glm::vec3(
		atan2(2.0f * (m_rot.w * m_rot.x - m_rot.y * m_rot.z), -2.0f * (m_rot.x * m_rot.x + m_rot.z * m_rot.z) + 1.0f),
		atan2(2.0f * (m_rot.w * m_rot.y - m_rot.x * m_rot.z), -2.0f * (m_rot.y * m_rot.y + m_rot.z * m_rot.z) + 1.0f),
		asin(2.0f * (m_rot.w * m_rot.z + m_rot.x * m_rot.y))
	);
}
glm::vec3 Transform::QuatToZXY() const  {
	return glm::vec3(
		asin(2.0f * (m_rot.w * m_rot.x + m_rot.y * m_rot.z)),
		atan2(2.0f * (m_rot.w * m_rot.y - m_rot.x * m_rot.z), -2.0f * (m_rot.x * m_rot.x + m_rot.y * m_rot.y) + 1.0f),
		atan2(2.0f * (m_rot.w * m_rot.z - m_rot.x * m_rot.y), -2.0f * (m_rot.x * m_rot.x + m_rot.z * m_rot.z) + 1.0f)
	);
}
glm::vec3 Transform::QuatToZYX() const  {
	return glm::vec3(
		atan2(2.0f * (m_rot.w * m_rot.x + m_rot.y * m_rot.z), -2.0f * (m_rot.x * m_rot.x + m_rot.y * m_rot.y) + 1.0f),
		asin(2.0f * (m_rot.w * m_rot.y - m_rot.x * m_rot.z)),
		atan2(2.0f * (m_rot.w * m_rot.z + m_rot.x * m_rot.y), -2.0f * (m_rot.y * m_rot.y + m_rot.z * m_rot.z) + 1.0f)
	);
}