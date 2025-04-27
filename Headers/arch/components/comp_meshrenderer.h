#pragma once
#include <arch/common/component.h>
#include <arch/resources/res_mesh.h>


class Material {
	
};

class MeshRenderer : Component {


public:
	MeshRenderer();




	void Init();
	void End();

private:

	// Needs Mesh.
	// model resource
	Mesh* m_mesh;
	std::vector<Material> m_materials;
	bool m_castShadow					{ true };
	bool m_staticShadow					{};
};