#pragma once
#include <arch/resources/res_mesh/res_mesh.h>
#include <graphics/resources/gfx_gpusubmesh.h>


class GPUMesh {
public:
	void Load(MeshRes& _mesh);
	void Update(MeshRes& _mesh);
	void Destroy();

	std::vector<GPUSubmesh>& GetGPUSubmeshList();
	const std::vector<GPUSubmesh>& GetGPUSubmeshList() const;
private:
	std::shared_ptr<MeshRes> m_meshPtr;
	std::vector<GPUSubmesh> m_submeshList;
	// handle is useless
};