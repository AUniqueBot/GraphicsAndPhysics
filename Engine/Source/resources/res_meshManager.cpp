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

	// -------------------------------------------------------------------------------------------

	RegisterTypenameToFunction(
		"Cube",
		[this](const Serialization::JSONValue& _val, ResourceHandle* _handle) {
			ResourceHandle handle = CreateCubeMesh();
			handle.GetBaseResource()->Deserialize(_val, nullptr);
			*_handle = handle;
		}
	);
	RegisterTypenameToFunction(
		"Plane",
		[this](const Serialization::JSONValue& _val, ResourceHandle* _handle) {
			ResourceHandle handle = CreatePlaneMesh();
			handle.GetBaseResource()->Deserialize(_val, nullptr);
			*_handle = handle;
		}
	);
	RegisterTypenameToFunction(
		"Sphere",
		[this](const Serialization::JSONValue& _val, ResourceHandle* _handle) {
			ResourceHandle handle = CreateSphereMesh();
			handle.GetBaseResource()->Deserialize(_val, nullptr);
			*_handle = handle;
		}
	);

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
	ResourceIdentifier idr = m_resourceManager.AddRes(mesh);
	MeshHandle handle(idr);
	AddResourceToPool(handle);
	return handle;
}

PlaneHandle MeshManager::CreatePlaneMesh(PlaneCreationProps _props) {
	std::shared_ptr<PlaneRes> mesh = std::make_shared<PlaneRes>(_props);
	mesh->Init();
	ResourceIdentifier idr = m_resourceManager.AddRes(mesh);
	PlaneHandle handle(RegisterResource(mesh));
	AddResourceToPool(handle);
	return handle;
}

CubeHandle MeshManager::GetDefaultCubeMesh() {
	const char* alias = VertexAttributeConstants::C_DEFAULT_MESH_CUBE;
	RES_ID defaultModel = m_resourceManager.GetResourceFromAlias(alias);
	return CubeHandle(m_resourceManager.GetResourceIdentifier(defaultModel));
	
}

SphereHandle MeshManager::GetDefaultSphereMesh() {
	const char* alias = VertexAttributeConstants::C_DEFAULT_MESH_SPHERE;
	RES_ID defaultModel = m_resourceManager.GetResourceFromAlias(alias);
	return SphereHandle(m_resourceManager.GetResourceIdentifier(defaultModel));
}

PlaneHandle MeshManager::GetDefaultPlaneMesh() {
	const char* alias = VertexAttributeConstants::C_DEFAULT_MESH_SPHERE;
	RES_ID defaultModel = m_resourceManager.GetResourceFromAlias(alias);
	return PlaneHandle(m_resourceManager.GetResourceIdentifier(defaultModel));
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
