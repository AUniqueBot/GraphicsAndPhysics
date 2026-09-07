#include <arch/resources/res_meshManager.h>
#include <arch/resources/res_mesh_presets/res_mesh_types.h>



#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

void MeshManager::Init() {
	RegisterFileExtension(".obj"); // should it ever be here?

	CubeHandle cube = CreateCubeMesh();
	PlaneHandle plane = CreatePlaneMesh();
	SphereHandle sphere = CreateSphereMesh();
	//MeshHandle icosphere = CreateCubeMesh();
	SetResourceAlias(cube.GetResourceID(), VertexAttributeConstants::C_DEFAULT_MESH_CUBE);
	SetResourceAlias(plane.GetResourceID(), VertexAttributeConstants::C_DEFAULT_MESH_PLANE);
	SetResourceAlias(sphere.GetResourceID(), VertexAttributeConstants::C_DEFAULT_MESH_SPHERE);
	//SetResourceAlias(icosphere.GetResourceID(), MeshConstants::C_DEFAULT_MESH_ICOSPHERE);
}



MeshHandle MeshManager::LoadMesh(std::filesystem::path _path, RES_ID _id) {
	std::shared_ptr<MeshRes> mesh = std::make_shared<MeshRes>();
	if (_id != ResourceConstants::C_RES_INVALID_ID) {
		mesh->ResourceID(_id);
	}
	// -------------------------------------------------------------------------------------------
	mesh->LoadMeshFromPath(_path);
	MeshHandle handle(RegisterResource(mesh));
	AddResourceToPool(handle);
	return handle;
}

CubeHandle MeshManager::CreateCubeMesh(CubeCreationProps _props) {
	std::shared_ptr<CubeRes> mesh = std::make_shared<CubeRes>(_props);	
	mesh->Init();
	CubeHandle handle(RegisterResource(mesh));
	AddResourceToPool(handle);
	return handle;
}

SphereHandle MeshManager::CreateSphereMesh(SphereCreationProps _props) {
	std::shared_ptr<SphereRes> mesh = std::make_shared<SphereRes>(_props);
	mesh->Init();
	SphereHandle handle(RegisterResource(mesh));
	AddResourceToPool(handle);
	return handle;
}

MeshHandle MeshManager::CreateIcosphereMesh() {
	std::shared_ptr<SphereRes> mesh = std::make_shared<SphereRes>();
	mesh->Init();
	ResourceIdentifier idr = m_resourceManager.AddInternalResource(mesh);
	MeshHandle handle(idr);
	AddResourceToPool(handle);
	return handle;
}

PlaneHandle MeshManager::CreatePlaneMesh(PlaneCreationProps _props) {
	std::shared_ptr<PlaneRes> mesh = std::make_shared<PlaneRes>(_props);
	mesh->Init();
	ResourceIdentifier idr = m_resourceManager.AddInternalResource(mesh);
	PlaneHandle handle(RegisterResource(mesh));
	AddResourceToPool(handle);
	return handle;
}

void MeshManager::LoadResource(const Serialization::MetafileData& _data) {
	namespace fs = std::filesystem;
	fs::path path = _data.path;
	if (!fs::exists(_data.path)) {
		LOG_WARN("Unable to open resource: " << _data.path);
		return;
	}
	MeshHandle mesh = LoadMesh(path, _data.id);
	mesh->ResourcePath(path);
	AddResourceToPool(mesh);
}

void MeshManager::UploadMeshToGPU(MeshRes& _mesh) {
	_mesh.SetGPUResourceHandle(m_gpuResourceManager.CreateMesh(_mesh));
}
