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




	void SetMesh(std::shared_ptr<Mesh> _mesh);
	std::shared_ptr<const Mesh> GetMesh() const;
	std::shared_ptr<Mesh> GetMesh();
	

	/*
		@brief
			gets material list
	*/
	std::vector<std::shared_ptr<Material>>& GetMaterialList()			{ return m_materials; }
	const std::vector<std::shared_ptr<Material>>& GetMaterialList() const { return m_materials; }

	void AddMaterial(std::shared_ptr<Material> _material);

	void Render(
		const glm::mat4& _objectMatrix, 
		const glm::mat4& _projectionMatrix, 
		const glm::mat4& _cameraMatrix
		);


	static Material& GetDefaultMaterial();



public:

private:

	// Needs Mesh.
	// model resource
	
	std::shared_ptr<Mesh> m_mesh		{};
	std::vector<std::shared_ptr<Material>> m_materials;

	bool m_castShadow					{ true };
	bool m_staticShadow					{};


};

