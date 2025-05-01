#pragma once

#include <arch/common/component.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>



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
	Transform();

	// --------------------------------------------------
	void SetPosition(glm::vec3 _pos)				{ m_pos = _pos; };
	glm::vec3 GetPosition() const					{ return m_pos; };

	void SetRotation(glm::quat _rot)				{ m_rot = _rot; };
	glm::quat GetRotation() const					{ return m_rot; };
	
	// euler rotations
	void SetRotationEuler(glm::vec3);
	glm::vec3 GetRotationEuler() const;
	void SetRotationOrder(ROTATION_ORDER _rotOrder) { m_rotOrder = _rotOrder; }
	ROTATION_ORDER GetRotationOrder() const			{ return m_rotOrder; }



	void SetScale(glm::vec3 _scale)					{ m_scl = _scale; };
	glm::vec3 GetScale() const						{ return m_scl; };


	// --------------------------------------------------
	glm::vec3 Forward() const;
	void Forward(glm::vec3 _newForward);

	glm::vec3 Up() const;
	void Up(glm::vec3 _newUp);

	glm::mat4 LocalTransformMtx() const;
	glm::mat4 LocalWorldMtx() const;
	void LocalTransformMtx(glm::mat4 _newMtx);
	void LocalWorldMtx(glm::mat4 _newMtx);





	static void Register() { std::cout << typeid(Transform).name() << ":Register override" << std::endl; }


private:
	glm::vec3 QuatToXYZ() const;
	glm::vec3 QuatToXZY() const;
	glm::vec3 QuatToYXZ() const;
	glm::vec3 QuatToYZX() const;
	glm::vec3 QuatToZXY() const;
	glm::vec3 QuatToZYX() const;

private:
	glm::vec3 m_pos		{};
	glm::quat m_rot		{};
	glm::vec3 m_scl		{};

	ROTATION_ORDER m_rotOrder		{ ROTATION_ORDER::XYZ };


};

