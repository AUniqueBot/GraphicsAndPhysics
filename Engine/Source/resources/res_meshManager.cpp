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
	SetResourceAlias(cube.GetResourceID(), MeshConstants::C_DEFAULT_MESH_CUBE);
	SetResourceAlias(plane.GetResourceID(), MeshConstants::C_DEFAULT_MESH_PLANE);
	SetResourceAlias(sphere.GetResourceID(), MeshConstants::C_DEFAULT_MESH_SPHERE);
	//SetResourceAlias(icosphere.GetResourceID(), MeshConstants::C_DEFAULT_MESH_ICOSPHERE);
}

MeshHandle MeshManager::LoadMesh(std::filesystem::path _path) {
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	// -------------------------------------------------------------------------------------------


	// general flow
	Assimp::Importer importer; // immediate triangulation.
	unsigned importerFlags = 
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
		;
	const aiScene* scene = importer.ReadFile(_path.string(), importerFlags);


	if (!scene || !scene->HasMeshes()) {
		LOG_INFO("Model failed to load");
		return MeshHandle(std::nullopt);
	}
	LOG_INFO("Loading Model.");

	unsigned meshCount = scene->mNumMeshes;
	aiMesh** meshList = scene->mMeshes;
	LOG_INFO(meshCount);
	



	// loading models
	// treat multiple models as single object.

	std::vector<float> vertexPosData{};
	std::vector<float> vertexNmlData{};
	std::vector<unsigned> faceIndexData{};
	unsigned vtxCount{};
	

	mesh->SetSubmeshCount(meshCount);
	for (unsigned meshIndex{}; meshIndex < meshCount; ++meshIndex) {

		const aiMesh* currentMesh = meshList[meshIndex];
		const unsigned _vtxCount = currentMesh->mNumVertices;
		const unsigned _idxCount = currentMesh->mNumFaces;


		unsigned currentMeshIndexOffset = vtxCount;
		for (unsigned int i = 0; i < currentMesh->mNumFaces; ++i) {
			const aiFace& face = currentMesh->mFaces[i];
			// typically faces are triangles
			for (unsigned int j = 0; j < face.mNumIndices; ++j) {
				faceIndexData.push_back(vtxCount + face.mIndices[j]);
			}
		}
		vtxCount += _vtxCount;

		// attach to the vertex pos data, nml data, etc.
		const float* posPtr =
			reinterpret_cast<const float*>(currentMesh->mVertices);

		vertexPosData.insert(
			vertexPosData.end(),
			posPtr,
			posPtr + (_vtxCount * 3)
		);
		const float* nmlPtr =
			reinterpret_cast<const float*>(currentMesh->mNormals);
		vertexNmlData.insert(
			vertexNmlData.end(),
			nmlPtr,
			nmlPtr + (_vtxCount * 3)
		);


		// add submeshes into the thing.
		mesh->AddSubmesh(
			Submesh::CreateSubmesh(*currentMesh, aiProcess_Triangulate & importerFlags)
		);

	}


	mesh->SetVertexPositions(
		reinterpret_cast<const glm::vec3*>(vertexPosData.data()),
		vtxCount
	);
	mesh->SetVertexNormals(
		reinterpret_cast<const glm::vec3*>(vertexNmlData.data()),
		vtxCount
	);
	mesh->SetIndices(
		faceIndexData.data(),
		static_cast<unsigned>(faceIndexData.size())
	);

	// loading mats
	unsigned matCount = scene->mNumMaterials;
	aiMaterial** matList = scene->mMaterials;


	// -------------------------------------------------------------------------------------------
	mesh->LoadMeshFromPath(_path);
	MeshHandle handle(RegisterResource(mesh));
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
