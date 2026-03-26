
#include <pch.h>
#include <arch/components/comp_transform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
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
	case RotationOrder_::XYZ:
		m_rot = qz * qy * qx;
		break;
	case RotationOrder_::XZY:
		m_rot = qy * qz * qx;
		break;
	case RotationOrder_::YXZ:
		m_rot = qz * qx * qy;
		break;
	case RotationOrder_::YZX:
		m_rot = qx * qz * qy;
		break;
	case RotationOrder_::ZXY:
		m_rot = qy * qx * qz;
		break;
	case RotationOrder_::ZYX:
		m_rot = qx * qy * qz;
		break;
	}
}

const glm::vec3& Transform::RotationEuler() const {
	return
		m_rotOrder == RotationOrder_::XYZ ? QuatToXYZ() :
		m_rotOrder == RotationOrder_::XZY ? QuatToXZY() :
		m_rotOrder == RotationOrder_::YXZ ? QuatToYXZ() :
		m_rotOrder == RotationOrder_::YZX ? QuatToYZX() :
		m_rotOrder == RotationOrder_::ZXY ? QuatToZXY() :
		m_rotOrder == RotationOrder_::ZYX ? QuatToZYX() : QuatToXYZ();
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
glm::mat4 Transform::WorldTransformMtx() const {
	return LocalTransformMtx();
}



glm::vec3 Transform::QuatToXYZ() const  {
	glm::mat4 quatCast{ glm::mat4_cast(m_rot) };
	glm::vec3 rot{};
	glm::extractEulerAngleXYZ(quatCast, rot.x, rot.y, rot.z);
	return glm::degrees(rot);
}
glm::vec3 Transform::QuatToXZY() const  {
	glm::mat4 quatCast{ glm::mat4_cast(m_rot) };
	glm::vec3 rot{};
	glm::extractEulerAngleXZY(quatCast, rot.x, rot.y, rot.z);
	return glm::degrees(rot);
}
glm::vec3 Transform::QuatToYXZ() const  {
	glm::mat4 quatCast{ glm::mat4_cast(m_rot) };
	glm::vec3 rot{};
	glm::extractEulerAngleYXZ(quatCast, rot.x, rot.y, rot.z);
	return glm::degrees(rot);
}
glm::vec3 Transform::QuatToYZX() const  {
	glm::mat4 quatCast{ glm::mat4_cast(m_rot) };
	glm::vec3 rot{};
	glm::extractEulerAngleYZX(quatCast, rot.x, rot.y, rot.z);
	return glm::degrees(rot);
}
glm::vec3 Transform::QuatToZXY() const  {
	glm::mat4 quatCast{ glm::mat4_cast(m_rot) };
	glm::vec3 rot{};
	glm::extractEulerAngleZXY(quatCast, rot.x, rot.y, rot.z);
	return glm::degrees(rot);
}
glm::vec3 Transform::QuatToZYX() const  {
	glm::mat4 quatCast{ glm::mat4_cast(m_rot) };
	glm::vec3 rot{};
	glm::extractEulerAngleZYX(quatCast, rot.x, rot.y, rot.z);
	return glm::degrees(rot);
}


// definition in cpp to reduce compile time.

std::vector<PropertyMD::Property>& Transform::GetProps() {
	using namespace PropertyMD;
	static std::vector<Property> props{
		PropertyMD::MakeProperty<Transform>("Position", PropertyType::Float, Shape::FixedArray, 3,
			static_cast<const glm::vec3 & (Transform::*)()const>(&Transform::Position),
			static_cast<void (Transform::*)(glm::vec3)>(&Transform::Position),
			true
			),
		PropertyMD::MakeProperty<Transform>("Rotation", PropertyType::Float, Shape::FixedArray, 3,
			static_cast<const glm::vec3 & (Transform::*)() const>(&Transform::RotationEuler),
			static_cast<void (Transform::*)(glm::vec3)>(&Transform::RotationEuler),
			true
			),
		PropertyMD::MakeProperty<Transform>("Scale", PropertyType::Float, Shape::FixedArray, 3,
			static_cast<const glm::vec3 & (Transform::*)() const>(&Transform::Scale),
			static_cast<void (Transform::*)(glm::vec3)>(&Transform::Scale)
		),
		PropertyMD::MakeProperty<Transform>("Quaternion", PropertyType::Float, Shape::FixedArray, 4,
			static_cast<const glm::quat & (Transform::*)() const>(&Transform::Rotation),
			static_cast<void (Transform::*)(glm::quat)>(&Transform::Rotation),
			true
		),
		PropertyMD::MakeEnumProperty<Transform, RotationOrder_>("Rotation Order", 
		static_cast<const RotationOrder_& (Transform::*)() const>(&Transform::RotationOrder),
		static_cast<void (Transform::*)(const RotationOrder_&) >(&Transform::RotationOrder),
			{
				{ "XYZ", static_cast<int>(RotationOrder_::XYZ) },
				{ "XZY", static_cast<int>(RotationOrder_::XZY) },
				{ "YXZ", static_cast<int>(RotationOrder_::YXZ) },
				{ "YZX", static_cast<int>(RotationOrder_::YZX) },
				{ "ZXY", static_cast<int>(RotationOrder_::ZXY) },
				{ "ZYX", static_cast<int>(RotationOrder_::ZYX) }

			}
			),
	};
	return props;
}