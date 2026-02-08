#pragma once
#include <arch/resources/res_mesh.h>

class Sphere : public Mesh {
private:
public:
	Sphere(float _radius = 0.5f, glm::ivec2 _subdivisions = { 8, 16 });
	void Init() override;

protected:
	void UpdateVertexData();
private:
	float m_radius				{ 0.5f };
	glm::ivec2 m_subdivisions	{ 8, 8 }; // width-wise
};