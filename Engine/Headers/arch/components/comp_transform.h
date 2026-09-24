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
	enum class RotationOrder_ {
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
	void Position(glm::vec3 _pos);
	const glm::vec3& Position() const;

	void Rotation(glm::quat _rot);
	const glm::quat& Rotation() const;
	
	void RotationEuler(glm::vec3);
	const glm::vec3& RotationEuler() const;

	void RotationOrder(const RotationOrder_& _rotOrder);
	const RotationOrder_& RotationOrder() const;

	void NormalizeRotation();


	void Scale(glm::vec3 _scale);
	const glm::vec3& Scale() const;

	
	// - orientation vectors ------------------
	glm::vec3 Forward() const;
	void Forward(glm::vec3 _newForward);

	glm::vec3 Up() const;
	void Up(glm::vec3 _newUp);

	// - transform matrix ----------------------
	glm::mat4 TransformMtx();
	glm::mat4 WorldTransformMtx(); // 
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

	glm::mat4 m_transformMtx{ 1.f };

	RotationOrder_ m_rotOrder		{ RotationOrder_::XYZ };
	bool m_mtxDirty					{ true };
	INSPECTABLE_DECLAREPROPS(Transform);

};


