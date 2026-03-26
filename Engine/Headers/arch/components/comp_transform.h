#pragma once

#include <arch/common/component.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>




struct alignas(sizeof(glm::vec4)) ObjectUBOData {
	glm::mat4 m_objectMatrix;
	glm::vec3 m_position;
	GLuint m_objectId;
};


class Transform : public Component {
public:
	enum class ROTATION_ORDER {
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX
	};
public:
	
	// - generic ------------------------------
	Transform();

	void Init() override;

	void End() override;

	// - transform attributes -----------------
	void Position(glm::vec3 _pos)					{ m_pos = _pos; };
	const glm::vec3& Position() const				{ return m_pos; };

	void Rotation(glm::quat _rot)					{ m_rot = glm::normalize(_rot); };
	const glm::quat& Rotation() const				{ return m_rot; };
	
	void RotationEuler(glm::vec3);
	const glm::vec3& RotationEuler() const;

	void RotationOrder(ROTATION_ORDER _rotOrder)	{ m_rotOrder = _rotOrder; }
	ROTATION_ORDER RotationOrder() const			{ return m_rotOrder; }

	void Scale(glm::vec3 _scale)					{ m_scl = _scale; };
	const glm::vec3& Scale() const					{ return m_scl; };

	
	// - orientation vectors ------------------
	glm::vec3 Forward() const;
	void Forward(glm::vec3 _newForward);

	glm::vec3 Up() const;
	void Up(glm::vec3 _newUp);

	// - transform matrix ----------------------
	glm::mat4 LocalTransformMtx() const;
	glm::mat4 WorldTransformMtx() const;
	void LocalTransformMtx(glm::mat4 _newMtx);
	void WorldTransformMtx(glm::mat4 _newMtx);





	static void Register() { LOG_INFO("Register Override"); }


private:
	// - rotation conversions ------------------
	glm::vec3 QuatToXYZ() const;
	glm::vec3 QuatToXZY() const;
	glm::vec3 QuatToYXZ() const;
	glm::vec3 QuatToYZX() const;
	glm::vec3 QuatToZXY() const;
	glm::vec3 QuatToZYX() const;

private:
	glm::vec3 m_pos					{};
	glm::quat m_rot					{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 m_scl					{ 1.0f, 1.0f, 1.0f };

	glm::mat4 m_transformMtx		{ 1.f };

	ROTATION_ORDER m_rotOrder		{ ROTATION_ORDER::XYZ };

	COMPONENT_DECLAREPROPS(Transform);

};


