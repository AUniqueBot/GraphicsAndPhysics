#pragma once
#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_gpu_resources/res_gpu_submesh.h>


class GPU_Mesh {
public:
	void Load(Mesh& _mesh);
	void Update(Mesh& _mesh);
	void Destroy();

	std::vector<GPU_Submesh>& GetGPUSubmeshList();
	const std::vector<GPU_Submesh>& GetGPUSubmeshList() const;
private:
	std::shared_ptr<Mesh> m_meshPtr;
	std::vector<GPU_Submesh> m_submeshList;
	// handle is useless
};