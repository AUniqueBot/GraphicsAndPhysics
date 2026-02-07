#pragma once
#include <arch/resources/res_mesh.h>



class Plane : public Mesh {
private:
public:
	Plane(glm::vec2 _dimensions = { 1.f, 1.f }, glm::ivec2 _subdivisions = {0, 0});

	void Init() override;
protected:
	void UpdateVertexData();
private:
	glm::vec2 m_dimensions				{ 1.f, 1.f };
	glm::ivec2 m_subdivisions			{ 0, 0 };
};