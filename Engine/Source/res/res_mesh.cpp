#include <pch.h>
#include <arch/resources/res_mesh.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags





// - class methods -------------------------------------

void Mesh::Load() {
	// general flow
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		m_pathToAsset.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType
	);


	if (!scene || !scene->HasMeshes()) {
		LOG_INFO("Model failed to load");
		return;
	}
	LOG_INFO("Loading Model.");
	
	unsigned meshCount	= scene->mNumMeshes;
	aiMesh** meshList	= scene->mMeshes;
	LOG_INFO(meshCount);
	

	
	// loading models
	// treat multiple models as single object.

	std::vector<float> vertexPosData	{};
	std::vector<float> vertexNmlData	{};
	std::vector<unsigned> faceIndexData	{};
	unsigned vtxCount	{};

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
	}


	SetVertexPositions(
		reinterpret_cast<const float*>(vertexPosData.data()),
		vtxCount
	);
	SetVertexNormals(
		reinterpret_cast<const float*>(vertexNmlData.data()),
		vtxCount
	);
	SetIndices(
		faceIndexData.data(),
		faceIndexData.size()
	);

	// loading mats
	unsigned matCount	= scene->mNumMaterials;
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






unsigned Mesh::GetVertexCount() const {
	return m_attributeData.at("position")->ElementCount() / 3; // pos = vec3 = 3x float
}


const float* Mesh::GetVertexData() const {
	return GetData<float>("position");
}


const size_t Mesh::GetVertexDataSize() const {
	return m_attributeData.at("position")->ElementCount() * sizeof(float);
}

const size_t Mesh::GetNormalDataSize() const {
	return m_attributeData.at("normal")->ElementCount() * sizeof(glm::vec3);
}
const float* Mesh::GetNormalData() const {
	return GetData<float>("normal");
}

const unsigned Mesh::GetUVCount() const {
	return m_uvs.size();
}

const size_t Mesh::GetUVDataSize(unsigned _index) const {
	if (_index >= m_uvs.size()) return 0;
	return m_uvs[_index].size() * sizeof(glm::vec2);
}

const float* Mesh::GetUVData(unsigned _index) const {
	if (_index >= m_uvs.size()) return nullptr;
	return reinterpret_cast<const float*>(m_uvs[_index].data());
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
void Mesh::SetVertexPositions(const float* _pointer, unsigned _vertexCount) {
	SetData<float>("position", _pointer, _vertexCount * sizeof(glm::vec3) / sizeof(float));
}



void Mesh::SetVertexNormals(const float* _pointer, unsigned _vertexCount) {
	SetData<float>("normal", _pointer, _vertexCount * sizeof(glm::vec3) / sizeof(float));
}


void Mesh::SetIndices(const unsigned* _pointer, unsigned _indexCount) {
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
	m_indices;
}

void Mesh::SetIndices(const glm::uvec3* _pointer, unsigned _indexGroupCount) {
	m_indices.assign(_pointer, _pointer + _indexGroupCount);
}







