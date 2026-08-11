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




	void SetMesh(RES_ID _mesh);
	RES_ID GetMesh() const;
	

	/*
		@brief
			gets material list
	*/
	std::vector<MaterialHandle>& GetMaterialList()				{ return m_materials; }
	const std::vector<MaterialHandle>& GetMaterialList() const	{ return m_materials; }

	void AddMaterial(MaterialHandle _material);
	void RemoveMaterial(MaterialHandle _material);
	



	static Material& GetDefaultMaterial();

	// - shadows ----------------------------------------------------------------------------------
	inline const bool& GetCastShadow() const			{ return m_castShadow; };
	inline void SetCastShadow(bool _cast)				{ m_castShadow = _cast; };

public:

private:

	// Needs Mesh.
	// model resource
	
	
	RES_ID m_mesh										{ BaseResource::C_RES_ID_INVALID };
	std::vector<MaterialHandle> m_materials; // change to material handle.
	

	bool m_castShadow									{ true };
	bool m_staticShadow									{}; // bake!


public:
	
	INSPECTABLE_DECLAREPROPS(MeshRenderer);
};

