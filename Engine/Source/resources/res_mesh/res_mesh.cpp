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

}

// - submesh methods ----------------------------------

void Submesh::ClearSubmeshInformation() {
	m_attributeData.clear();
	m_indices.clear();
	m_vertexCount = 0;
}

// -- vertex ---------------
void Submesh::SetVertexCount(size_t _vtxCount) {
	m_vertexCount = _vtxCount;
}


size_t Submesh::GetVertexCount() const {
	return m_vertexCount;
}

const glm::vec3* Submesh::GetVertexPositions() const {
	return GetData<glm::vec3>(MeshConstants::C_VTXATTR_POSITION);
}

void Submesh::SetVertexPositions(const glm::vec3* _pointer) {
	// mesh assumes the vertex count provided is correct.
	SetData<glm::vec3>(MeshConstants::C_VTXATTR_POSITION, _pointer, m_vertexCount);
}

const size_t Submesh::GetVertexDataSize() const {
	return m_attributeData.at(MeshConstants::C_VTXATTR_POSITION)->ElementCount() * sizeof(glm::vec3);
}



// -- normal ---------------
void Submesh::SetVertexNormals(const glm::vec3* _pointer) {
	SetData<glm::vec3>(MeshConstants::C_VTXATTR_NORMAL, _pointer, m_vertexCount);
}
const size_t Submesh::GetNormalDataSize() const {
	return m_attributeData.at(MeshConstants::C_VTXATTR_NORMAL)->ElementCount() * sizeof(glm::vec3);
}
const glm::vec3* Submesh::GetNormalData() const {
	return GetData<glm::vec3>(MeshConstants::C_VTXATTR_NORMAL);
}


// 
void Submesh::SetVertexUVs(unsigned _index, const glm::vec2* _pointer) {
	std::string uvId = MeshConstants::C_VTXATTR_UV + _index;
	SetData<glm::vec2>(uvId, _pointer, m_vertexCount);
}

const size_t Submesh::GetUVDataSize(unsigned _index) const {
	std::string uvId = MeshConstants::C_VTXATTR_UV + _index;
	return m_attributeData.at(uvId)->ElementCount() * sizeof(glm::uvec2);

}
const glm::uvec2* Submesh::GetUVData(unsigned _index) const {
	std::string uvId = MeshConstants::C_VTXATTR_UV + _index;
	return GetData<glm::uvec2>(uvId);
}

void Submesh::SetVertexIndices(const glm::uvec3* _pointer, size_t _faceCount) {
	m_indices.assign(_pointer, _pointer + _faceCount);
}

const glm::uvec3* Submesh::GetVertexIndexData() const {
	return m_indices.data();
}
size_t Submesh::GetVertexIndexCount() const {
	return m_indices.size();
}

const RES_ID& Submesh::GetMaterialID() const {
	return m_materialId;
}

void Submesh::SetMaterialID(const RES_ID& _id){
	m_materialId = _id;
}


VertexAttributeDatabase* Submesh::GetDatabase(const std::string& _name) {
	auto it = m_attributeData.find(_name);
	return it == m_attributeData.end() ? nullptr : it->second.get();
}
const VertexAttributeDatabase* Submesh::GetDatabase(const std::string& _name) const {
	auto it = m_attributeData.find(_name);
	return it == m_attributeData.end() ? nullptr : it->second.get();
}


AttributeData& Submesh::GetVertexInformation() {
	return m_attributeData;
}
const AttributeData& Submesh::GetVertexInformation() const {
	return m_attributeData;
}


Submesh Submesh::CreateSubmesh(const aiMesh& _mesh, bool _isTriangulated) {
	// for now 
	Submesh submesh;
	if (!_isTriangulated) {
		return submesh;
	}

	size_t vertexCount = _mesh.mNumVertices;
	unsigned uvCount = _mesh.GetNumUVChannels();
	submesh.SetVertexCount(vertexCount);
	submesh.SetVertexPositions(reinterpret_cast<const glm::vec3*>(_mesh.mVertices));

	if (_mesh.HasNormals()) {
		submesh.SetVertexNormals(reinterpret_cast<const glm::vec3*>(_mesh.mNormals));
	}

	if (_mesh.HasFaces()) {
		std::vector<glm::uvec3> faceIndices = GetFaceIndices(_mesh, _isTriangulated);
		submesh.SetVertexIndices(faceIndices.data(), faceIndices.size());
	}

	// - uvs ---------------------------------------

	if (uvCount) {
		// assume we only have 1 UV set.
		int id{};
		int uvCompSize = *_mesh.mNumUVComponents;
		int validUvCount = 0;
		// flagged for potential improvement.
		for (unsigned uvCh{}; uvCh < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++uvCh) {
			if (_mesh.HasTextureCoords(uvCh)) {
				++validUvCount;
				std::string attrName = MeshConstants::C_VTXATTR_UV + id;
				const glm::vec3* currentUvData = reinterpret_cast<const glm::vec3*>(_mesh.mTextureCoords[uvCh]);
				if (uvCompSize == 2) {
					std::vector<glm::vec2> uvData(vertexCount);
					for (size_t i{}; i < vertexCount; ++i) {
						uvData[i].x = currentUvData[i].x;
						uvData[i].y = currentUvData[i].y;
					}
					submesh.SetData(attrName, uvData.data(), submesh.GetVertexCount());
				}
				else {
					submesh.SetData(attrName, currentUvData, submesh.GetVertexCount());
				}
			}
			// if reached uvCount you can stop.
			if (validUvCount == uvCount) break;
		}
	}



	return submesh;
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
	return m_attributeData.at(MeshConstants::C_VTXATTR_POSITION)->ElementCount() / 3; // pos = vec3 = 3x float
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
	SetData<glm::vec3>(MeshConstants::C_VTXATTR_POSITION, _pointer, _vertexCount);
}

void Mesh::SetVertexNormals(const glm::vec3* _pointer, size_t _vertexCount) {
	SetData<glm::vec3>(MeshConstants::C_VTXATTR_NORMAL, _pointer, _vertexCount);
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






