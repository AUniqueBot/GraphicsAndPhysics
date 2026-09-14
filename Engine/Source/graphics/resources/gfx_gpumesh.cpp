#include <pch.h>
#include <graphics/resources/gfx_gpumesh.h>

void GPUMesh::Load(MeshRes& _mesh) {
	for (const Submesh& submesh : _mesh.GetSubmeshList()) {
		GPUSubmesh gpuSubmesh;
		gpuSubmesh.Load(submesh);
		m_submeshList.push_back(gpuSubmesh);
	}
}

void GPUMesh::Update(MeshRes& _mesh) {
	for (const Submesh& submesh : _mesh.GetSubmeshList()) {
		GPUSubmesh gpuSubmesh;
		gpuSubmesh.Load(submesh);
		m_submeshList.push_back(gpuSubmesh);
	}
}


void GPUMesh::Destroy() {
	for (GPUSubmesh& submesh : m_submeshList) {
		submesh.Destroy();
	}
}

std::vector<GPUSubmesh>& GPUMesh::GetGPUSubmeshList() {
	return m_submeshList;
}

const std::vector<GPUSubmesh>& GPUMesh::GetGPUSubmeshList() const{
	return m_submeshList;
}
