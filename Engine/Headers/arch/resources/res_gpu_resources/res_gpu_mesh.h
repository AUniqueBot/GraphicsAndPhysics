#pragma once
#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_gpu_resources/res_gpu_submesh.h>


class GPU_Mesh {
public:
	void Load(MeshRes& _mesh);
	void Update(MeshRes& _mesh);
	void Destroy();

	std::vector<GPU_Submesh>& GetGPUSubmeshList();
	const std::vector<GPU_Submesh>& GetGPUSubmeshList() const;
private:
	std::shared_ptr<MeshRes> m_meshPtr;
	std::vector<GPU_Submesh> m_submeshList;
	// handle is useless
};