/*
	@file
	@author
	@brief component handling mesh rendering.
*/

#pragma once
#include <arch/common/component.h>
#include <arch/resources/res_mesh.h>
#include <arch/resources/res_material.h>






class MeshRenderer : public Component {


public:
	MeshRenderer();

	void Init();
	void End();




	int VAO() const;
	int VBO() const;


	const Mesh& GetMesh() const	{ return m_mesh; };
	Mesh& GetMesh()				{ return m_mesh; };
	

	/*
		@brief
			gets material list
	*/
	std::vector<Material>& GetMaterialList() { return m_materials; }
	const std::vector<Material>& GetMaterialList() const { return m_materials; }


	static Material& GetDefaultMaterial();


public:

private:

	// Needs Mesh.
	// model resource
	

	
	
	Mesh m_mesh						{};
	std::vector<Material> m_materials; // don't think abou this for a sec.
	bool m_castShadow					{ true };
	bool m_staticShadow					{};







};

