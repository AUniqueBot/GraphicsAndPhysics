#pragma once
#include <arch/resources/res_mesh.h>

class Sphere : public Mesh {
private:
public:
	void Init() override;

	void UpdateVertexData();
private:
	float m_radius				{ 0.5f };
	glm::ivec2 m_subdivisions	{ 4, 4 };
};