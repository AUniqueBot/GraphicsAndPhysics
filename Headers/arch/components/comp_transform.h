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
	
	// - generic ------------------------------
	
	();
	void Init() override;

	void End() override;

	// - transform attributes -----------------
	void Position(glm::vec3 _pos)					{ m_pos = _pos; };
	glm::vec3 Position() const						{ return m_pos; };

	void Rotation(glm::quat _rot)					{ m_rot = _rot; };
	glm::quat Rotation() const						{ return m_rot; };
	
	void RotationEuler(glm::vec3);
	glm::vec3 RotationEuler() const;

	void RotationOrder(ROTATION_ORDER _rotOrder)	{ m_rotOrder = _rotOrder; }
	ROTATION_ORDER RotationOrder() const			{ return m_rotOrder; }

	void Scale(glm::vec3 _scale)					{ m_scl = _scale; };
	glm::vec3 Scale() const							{ return m_scl; };

	
	// - orientation vectors ------------------
	glm::vec3 Forward() const;
	void Forward(glm::vec3 _newForward);

	glm::vec3 Up() const;
	void Up(glm::vec3 _newUp);

	// - transform matrix ----------------------
	glm::mat4 LocalTransformMtx() const;
	glm::mat4 WorldTransformdMtx() const;
	void LocalTransformMtx(glm::mat4 _newMtx);
	void WorldTransformdMtx(glm::mat4 _newMtx);





	static void Register() { std::cout << typeid(Transform).name() << ":Register override" << std::endl; }


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
	glm::quat m_rot					{};
	glm::vec3 m_scl					{};

	glm::mat4 m_transformMtx		{ 1.f };

	ROTATION_ORDER m_rotOrder		{ ROTATION_ORDER::XYZ };


};


