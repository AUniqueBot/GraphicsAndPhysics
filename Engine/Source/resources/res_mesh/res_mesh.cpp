#include <pch.h>
#include <arch/resources/res_mesh/res_mesh.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


namespace {
	std::vector<glm::uvec3> GetFaceIndices(const aiMesh& _mesh, bool _triangulated) {
		std::vector<glm::uvec3> indices;
		unsigned faceCount = _mesh.mNumFaces;
		const aiFace* faceArr = _mesh.mFaces;
		if (_triangulated) {
			indices.reserve(faceCount);
			for (unsigned i{}; i < faceCount; ++i) {
				indices.push_back(glm::uvec3(
					faceArr[i].mIndices[0],
					faceArr[i].mIndices[1],
					faceArr[i].mIndices[2]
				));
			}
		}
		else {

			for (unsigned i{}; i < faceCount; ++i) {
				unsigned trisCount = faceArr[i].mNumIndices - 2;
				for (unsigned tris{}; tris < trisCount; ++tris) {
					// index picking algorithm
				}
			}
		}
		return indices;
	}



	constexpr const char* GetUVVertexAttributeConstant(int _index) {
		return
			_index == 0 ? VertexAttributeConstants::C_VTXATTR_UV0 :
			_index == 1 ? VertexAttributeConstants::C_VTXATTR_UV1 :
			_index == 2 ? VertexAttributeConstants::C_VTXATTR_UV2 :
			_index == 3 ? VertexAttributeConstants::C_VTXATTR_UV3 :
			_index == 4 ? VertexAttributeConstants::C_VTXATTR_UV4 :
			_index == 5 ? VertexAttributeConstants::C_VTXATTR_UV5 :
			_index == 6 ? VertexAttributeConstants::C_VTXATTR_UV6 :
			_index == 7 ? VertexAttributeConstants::C_VTXATTR_UV7 :
			VertexAttributeConstants::C_VTXATTR_UV0;
	}

}

// - mesh methods -------------------------------------



void Mesh::Load() {


	// general flow
	Assimp::Importer importer; // immediate triangulation.
	unsigned importerFlags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
		;
	
	const aiScene* scene = importer.ReadFile(ResourcePath().string(), importerFlags);
	if (!scene || !scene->HasMeshes()) {
		LOG_INFO("Model failed to load");
		return;
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


	SetSubmeshCount(meshCount);
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
		AddSubmesh(
			Submesh::CreateSubmesh(*currentMesh, aiProcess_Triangulate & importerFlags)
		);

	}


	SetVertexPositions(
		reinterpret_cast<const glm::vec3*>(vertexPosData.data()),
		vtxCount
	);
	SetVertexNormals(
		reinterpret_cast<const glm::vec3*>(vertexNmlData.data()),
		vtxCount
	);
	SetIndices(
		faceIndexData.data(),
		static_cast<unsigned>(faceIndexData.size())
	);

	// loading mats
	unsigned matCount = scene->mNumMaterials;
	aiMaterial** matList = scene->mMaterials;


}

void Mesh::Init() {
	LoadAsset();
}

void Mesh::LoadAsset() {
	if (!BaseResource::IsAssetLoaded()) {
		BaseResource::LoadAsset();
		Load();
		LOG_INFO("mesh loaded asset.");
	}
}

void Mesh::LoadMeshFromPath(std::filesystem::path _pathToModel) {
	if (BaseResource::IsAssetLoaded()) {
		BaseResource::UnloadAsset();	
	}
	ResourcePath(_pathToModel);
	Name(_pathToModel.filename().string());
	Load();
}






size_t Mesh::GetVertexCount() const {
	return m_attributeData.at(VertexAttributeConstants::C_VTXATTR_POSITION)->ElementCount() / 3; // pos = vec3 = 3x float
}

void Mesh::SetSubmeshCount(size_t _count) {
	m_submeshList.reserve(_count);
}

size_t Mesh::GetSubmeshCount() const {
	return m_submeshList.capacity();
}

void Mesh::AddSubmesh(Submesh&& _smesh) {
	size_t currentCapacity = GetSubmeshCount();
	if (currentCapacity == m_submeshList.size()) {
		SetSubmeshCount(currentCapacity);
	}
	m_submeshList.emplace_back(std::move(_smesh));
}

Submesh& Mesh::GetSubmesh(int _idx) {
	return m_submeshList[_idx];
}
const Submesh& Mesh::GetSubmesh(int _idx) const {
	return m_submeshList[_idx];
}

std::vector<Submesh>& Mesh::GetSubmeshList() {
	return m_submeshList;
}

const std::vector<Submesh>& Mesh::GetSubmeshList() const {
	return m_submeshList;
}

const size_t Mesh::GetIndexDataSize() const {
	return m_indices.size() * sizeof(glm::ivec3);
}

const glm::uvec3* Mesh::GetIndexData() const {
	return m_indices.data();
}

const size_t Mesh::GetIndexDataCount() const {
	return m_indices.size();
}


std::string Mesh::VAOIdentifier() const {
	return m_vaoName;
}

void Mesh::VAOIdentifier(std::string& _newIdentifier) {
	if (m_vaoName == _newIdentifier) return;
	m_vaoName = _newIdentifier;
}


void Mesh::ClearMeshInformation() {
	m_attributeData.clear();
	m_uvs.clear();
}

// the
void Mesh::SetVertexPositions(const glm::vec3* _pointer, size_t _vertexCount) {
	SetData<glm::vec3>(VertexAttributeConstants::C_VTXATTR_POSITION, _pointer, _vertexCount);
}

void Mesh::SetVertexNormals(const glm::vec3* _pointer, size_t _vertexCount) {
	SetData<glm::vec3>(VertexAttributeConstants::C_VTXATTR_NORMAL, _pointer, _vertexCount);
}


void Mesh::SetIndices(const unsigned* _pointer, size_t _indexCount) {
	// 1 face == 3 verts.
	size_t faceGrpCount{ _indexCount / 3  };
	m_indices.resize(faceGrpCount);
	for (unsigned i{}; i < faceGrpCount; ++i) {
		unsigned offset{ i * 3 };
		m_indices[i] = glm::uvec3{
			_pointer[offset + 0],
			_pointer[offset + 1],
			_pointer[offset + 2]
		};
	}
}

void Mesh::SetIndices(const glm::uvec3* _pointer, size_t _indexGroupCount) {
	m_indices.assign(_pointer, _pointer + _indexGroupCount);
}

VertexAttributeDatabase* Mesh::GetDatabase(const std::string& _name) {
	auto it = m_attributeData.find(_name);
	return it == m_attributeData.end() ? nullptr : it->second.get();
}

const VertexAttributeDatabase* Mesh::GetDatabase(const std::string& _name) const {
	auto it = m_attributeData.find(_name);
	return it == m_attributeData.end() ? nullptr : it->second.get();
}

AttributeData& Mesh::GetVertexInformation() {
	return m_attributeData;
}

const AttributeData& Mesh::GetVertexInformation() const {
	return m_attributeData;
}






