#include <pch.h>
#include <arch/resources/res_gpu_resources/res_gpu_mesh.h>

void GPU_Mesh::Load(Mesh& _mesh) {
	for (const Submesh& submesh : _mesh.GetSubmeshList()) {
		GPU_Submesh gpuSubmesh;
		gpuSubmesh.Load(submesh);
		m_submeshList.push_back(gpuSubmesh);
	}
}

void GPU_Mesh::Update(Mesh& _mesh) {
	for (const Submesh& submesh : _mesh.GetSubmeshList()) {
		GPU_Submesh gpuSubmesh;
		gpuSubmesh.Load(submesh);
		m_submeshList.push_back(gpuSubmesh);
	}
}


void GPU_Mesh::Destroy() {
	for (GPU_Submesh& submesh : m_submeshList) {
		submesh.Destroy();
	}
}

std::vector<GPU_Submesh>& GPU_Mesh::GetGPUSubmeshList() {
	return m_submeshList;
}

const std::vector<GPU_Submesh>& GPU_Mesh::GetGPUSubmeshList() const{
	return m_submeshList;
}
