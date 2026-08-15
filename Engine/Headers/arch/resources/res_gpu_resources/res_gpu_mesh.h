#pragma once
#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_gpu_resources/res_gpu_submesh.h>


class GPU_Mesh {
public:
	void Load(Mesh& _mesh);
	void Destroy();
private:
	std::vector<GPU_Submesh> m_submeshList;
	// handle is useless
};