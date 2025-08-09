/*
	@file
	@author
	@brief component handling mesh rendering.
*/

#pragma once
#include <pch.h>
#include <arch/common/component.h>
#include <arch/resources/res_mesh.h>
#include <arch/resources/res_material.h>






class MeshRenderer : public Component {


public:
	MeshRenderer();

	void Init();
	void End();





	const Mesh& GetMesh() const	{ return m_mesh; };
	Mesh& GetMesh()				{ return m_mesh; };
	

	/*
		@brief
			gets material list
	*/
	std::vector<Material>& GetMaterialList()			{ return m_materials; }
	const std::vector<Material>& GetMaterialList() const { return m_materials; }


	void Render(
		const glm::vec4& _objectMatrix, 
		const glm::vec4& _projectionMatrix, 
		const glm::vec4& _cameraMatrix
		);


	static Material& GetDefaultMaterial();


public:

private:

	// Needs Mesh.
	// model resource
	
	Mesh m_mesh							{};
	std::vector<Material> m_materials;
	bool m_castShadow					{ true };
	bool m_staticShadow					{};


};

