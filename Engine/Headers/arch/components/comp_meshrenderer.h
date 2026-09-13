/*
	@file
	@author
	@brief component handling mesh rendering.
*/

#pragma once
#include <pch.h>
#include <arch/common/component.h>
#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_material.h>
#include <arch/systems/sys_render_modules/sys_render_shadowMap.h>





class MeshRenderer : public Component {


public:
	MeshRenderer();

	void Init();
	void End();




	void SetMesh(const MeshHandle& _mesh);
	MeshHandle& GetMesh();
	const MeshHandle& GetMesh() const;
	

	/*
		@brief
			gets material list
	*/
	std::vector<MaterialHandle>& GetMaterialList();
	const std::vector<MaterialHandle>& GetMaterialList() const;

	void AddMaterial(MaterialHandle _material);
	void RemoveMaterial(MaterialHandle _material);
	void RemoveMaterial(int _material);
	
	MaterialHandle GetMaterial(int _index);
	const MaterialHandle GetMaterial(int _index) const;


	static MaterialRes& GetDefaultMaterial();

	// - shadows ----------------------------------------------------------------------------------
	const bool& CastShadows() const;
	void CastShadows(const bool& _setting);

	const bool& ReceiveShadows() const;
	void ReceiveShadows(const bool& _setting);

	const bool& StaticShadows() const;
	void StaticShadows(const bool& _setting);

public:


private:

	// Needs Mesh.
	// model resource
	
	
	MeshHandle m_mesh									{ std::nullopt };
	std::vector<MaterialHandle> m_materials; // change to material handle.
	

	bool m_castShadows									{ true };
	bool m_receiveShadows								{ true };
	bool m_staticShadows								{ false }; // bake!


public:
	
	INSPECTABLE_DECLAREPROPS(MeshRenderer);
};

