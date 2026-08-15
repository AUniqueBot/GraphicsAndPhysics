#include <arch/resources/res_meshManager.h>
#include <arch/resources/res_mesh_presets/res_mesh_types.h>



#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

void MeshManager::Init() {
	CubeHandle cube = CreateCubeMesh();
	PlaneHandle plane = CreatePlaneMesh();
	SphereHandle sphere = CreateSphereMesh();
	//MeshHandle icosphere = CreateCubeMesh();
	SetResourceAlias(cube.GetResourceID(), VertexAttributeConstants::C_DEFAULT_MESH_CUBE);
	SetResourceAlias(plane.GetResourceID(), VertexAttributeConstants::C_DEFAULT_MESH_PLANE);
	SetResourceAlias(sphere.GetResourceID(), VertexAttributeConstants::C_DEFAULT_MESH_SPHERE);
	//SetResourceAlias(icosphere.GetResourceID(), MeshConstants::C_DEFAULT_MESH_ICOSPHERE);
}



MeshHandle MeshManager::LoadMesh(std::filesystem::path _path) {
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	// -------------------------------------------------------------------------------------------
	mesh->LoadMeshFromPath(_path);
	MeshHandle handle(RegisterResource(mesh));
	GPUResourceHandle gpuHandle = m_gpuResourceManager.CreateMesh(*mesh);
	return handle;
}

CubeHandle MeshManager::CreateCubeMesh(CubeCreationProps _props) {
	std::shared_ptr<Cube> mesh = std::make_shared<Cube>(_props);	
	mesh->Init();
	CubeHandle handle(RegisterResource(mesh));


	return handle;
}

SphereHandle MeshManager::CreateSphereMesh(SphereCreationProps _props) {
	std::shared_ptr<Sphere> mesh = std::make_shared<Sphere>(_props);
	mesh->Init();
	SphereHandle handle(RegisterResource(mesh));
	return handle;
}

MeshHandle MeshManager::CreateIcosphereMesh() {
	std::shared_ptr<Sphere> mesh = std::make_shared<Sphere>();
	mesh->Init();
	ResourceIdentifier idr = m_resourceManager.AddInternalResource(mesh);
	MeshHandle handle(idr);
	Add(handle.GetResourceID());
	return handle;
}

PlaneHandle MeshManager::CreatePlaneMesh(PlaneCreationProps _props) {
	std::shared_ptr<Plane> mesh = std::make_shared<Plane>(_props);
	mesh->Init();
	ResourceIdentifier idr = m_resourceManager.AddInternalResource(mesh);
	PlaneHandle handle(idr);
	Add(handle.GetResourceID());
	return handle;
}
