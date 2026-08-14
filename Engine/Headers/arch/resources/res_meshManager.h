#pragma once
#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_mesh_presets/res_mesh_types.h>
#include <arch/resources/res_specializedResourceManager.h>


namespace VertexAttributeConstants {
	inline constexpr const char* C_DEFAULT_MESH_CUBE { "C_DEFAULT_MESH_CUBE" };
	inline constexpr const char* C_DEFAULT_MESH_PLANE { "C_DEFAULT_MESH_PLANE" };
	inline constexpr const char* C_DEFAULT_MESH_SPHERE { "C_DEFAULT_MESH_SPHERE" };
	inline constexpr const char* C_DEFAULT_MESH_ICOSPHERE { "C_DEFAULT_MESH_ICOSPHERE" };
}



class MeshManager : public SpecializedResourceManager {
public:
	MeshManager(ResourceManager& _rsm, GPUResourceManager& _gpuMgr) : SpecializedResourceManager(_rsm, _gpuMgr) {}

public:
	void Init() override;

	MeshHandle LoadMesh(std::filesystem::path _path);
	
	// creates new instances of meshes.
	CubeHandle CreateCubeMesh(CubeCreationProps _props = CubeCreationProps()); 
	SphereHandle CreateSphereMesh(SphereCreationProps _props = SphereCreationProps());
	MeshHandle CreateIcosphereMesh(); 
	PlaneHandle CreatePlaneMesh(PlaneCreationProps _props = PlaneCreationProps());
	
};