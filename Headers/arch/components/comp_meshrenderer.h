/*
	@file
	@author
	@brief component handling mesh rendering.
*/

#pragma once
#include <arch/common/component.h>
#include <arch/resources/res_mesh.h>


class Material {
	glm::vec4 m_diffuseColor		{ HexToVec4(0xcececeff) };
};

class MeshRenderer : public Component {


public:
	MeshRenderer();

	void Init();
	void End();




	int VAO() const;
	int VBO() const;


public:

private:

	// Needs Mesh.
	// model resource
	

	
	
	Mesh m_mesh						{};
	std::vector<Material> m_materials;
	bool m_castShadow					{ true };
	bool m_staticShadow					{};







};

