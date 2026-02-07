#pragma once
#include <arch/resources/res_mesh.h>


class Cube : public Mesh {
private:
public:
	Cube(
		glm::vec3 _dimensions = { 1.f, 1.f, 1.f }, 
		glm::ivec3 _subdivisions = { 0, 0, 0 }
	);


	void Init() override;
protected:
	void UpdateVertexData();

private:
	glm::vec3 m_dimensions					{ 1.f, 1.f, 1.f };	// generate by face
	glm::ivec3 m_subdivisions				{ 0, 0, 0 }; // default of 0; no subdivisions.
};