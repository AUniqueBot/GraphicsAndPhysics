#pragma once
#include "../common/component.h"


class Material {
	
};

class MeshRenderer : Component {


public:
	MeshRenderer();

	void Init();
	void End();

private:
	std::vector<Material> m_materials;
	bool m_castShadow					{ true };
	bool m_staticShadow					{};
};