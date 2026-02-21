#pragma once
#include <pch.h>


class PhysicsEnvironmentSetup {




private:
	glm::vec3 m_gravity			{ 0.f, -9.81f, 0.f };
	glm::vec3 m_force			{};
	float m_dragCoefficient		{};
};