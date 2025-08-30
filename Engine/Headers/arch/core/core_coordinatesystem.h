#pragma once
#include <pch.h>


class Entity;
class CoordinateSystem {
private:

public:
	enum DIRECTION {
		X_POS,
		Y_POS,
		Z_POS,
		X_NEG,
		Y_NEG,
		Z_NEG
	};

public:
	// ctor
	CoordinateSystem() = default;
	~CoordinateSystem() = default;



	// world 
	inline glm::vec3 GetWorldUp()				const;
	inline glm::vec3 GetWorldForward()			const;


	// local
	// based on currently selected object, return local forward and up.
	glm::vec3 GetLocalUp(const Entity&)			const;
	glm::vec3 GetLocalFowrard(const Entity&)	const;


	// screen
	glm::vec3 GetScreenUp()						const;
	glm::vec3 GetScreenForward()				const;
	glm::mat4 GetScreenSpaceMatrix()			const;


private:
	DIRECTION m_world_forward				{ Z_NEG };
	DIRECTION m_world_up					{ Y_POS };

	// set active camera.

};