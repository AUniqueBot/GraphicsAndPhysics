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
#include <arch/systems/sys_render_modules/sys_render_shadowMap.h>





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
	std::vector<std::shared_ptr<Material>>& GetMaterialList()				{ return m_materials; }
	const std::vector<std::shared_ptr<Material>>& GetMaterialList() const	{ return m_materials; }

	void AddMaterial(std::shared_ptr<Material> _material);
	void RemoveMaterial(std::shared_ptr<Material> _material);
	void ApplyShadowMap(const ShadowMap& _shadowMap) const;

	void Render();



	static Material& GetDefaultMaterial();

	// - shadows ----------------------------------------------------------------------------------
	const bool& GetCastShadow() const	{ return m_castShadow; };
	void SetCastShadow(bool _cast)		{ m_castShadow = _cast; };

public:

private:

	// Needs Mesh.
	// model resource
	
	std::shared_ptr<Mesh> m_mesh		{};
	std::vector<std::shared_ptr<Material>> m_materials;

	bool m_castShadow					{ true };
	bool m_staticShadow					{};


};

