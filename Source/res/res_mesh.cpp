#include <pch.h>
#include <arch/resources/res_mesh.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

// - statics -------------------------------------------

static float positionData[] = {
	// x y z.

	// back face
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	// front face
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	// left face
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	// right face
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f, -0.5f,
	0.5f,  0.5f,  0.5f,
	// bottom face
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	// top face
	-0.5f,  0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	0.5f,  0.5f,  0.5f
};

static float normalData[] = {
	// x y z.

	// back face
	0.0f, 0.0f, -1.0f,  
	0.0f, 0.0f, -1.0f,  
	0.0f, 0.0f, -1.0f,  
	0.0f, 0.0f, -1.0f,  
	// front face
	0.0f, 0.0f,  1.0f,  
	0.0f, 0.0f,  1.0f,  
	0.0f, 0.0f,  1.0f,  
	0.0f, 0.0f,  1.0f,  
	// left face
	-1.0f, 0.0f, 0.0f,  
	-1.0f, 0.0f, 0.0f,  
	-1.0f, 0.0f, 0.0f,  
	-1.0f, 0.0f, 0.0f,  
	// right face
	1.0f, 0.0f, 0.0f,   
	1.0f, 0.0f, 0.0f,   
	1.0f, 0.0f, 0.0f,   
	1.0f, 0.0f, 0.0f,   
	// bottom face
	0.0f, -1.0f, 0.0f,  
	0.0f, -1.0f, 0.0f,  
	0.0f, -1.0f, 0.0f,  
	0.0f, -1.0f, 0.0f,  
	// top face
	0.0f,  1.0f, 0.0f,  
	0.0f,  1.0f, 0.0f,  
	0.0f,  1.0f, 0.0f,  
	0.0f,  1.0f, 0.0f  
};

static float uvData[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
};

static unsigned int idxList[] = { // note that we start from 0!
	0, 1, 3, // first triangle
	0, 2, 3, // second triangle

	4, 5, 7,
	4, 6, 7,

	8, 9, 11,
	8, 10, 11,

	12, 13, 15,
	12, 14, 15,

	16, 17, 19,
	16, 18, 19,

	20, 21, 23,
	20, 22, 23
};

// - class methods -------------------------------------

void Mesh::Load() {

	// Init();

	// general flow
	Assimp::Importer importer;
	std::string path{ "./Assets/Models/sampleModel.obj" };
	const aiScene* scene = importer.ReadFile(
		path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType

	);


	if (!scene || !scene->HasMeshes()) return;
	LOG_INFO("Loading Model.");
	
	unsigned meshCount	= scene->mNumMeshes;
	aiMesh** meshList	= scene->mMeshes;
	LOG_INFO(meshList);
	LOG_INFO(meshCount);
	

	
	// loading models
	

	
	for (unsigned meshIndex = 0; meshIndex < 1; ++meshIndex) {
		m_vertexPositions.clear();
		m_vertexNormals.clear();
		const aiMesh* currentMesh = meshList[meshIndex];

		const unsigned _vtxCount = currentMesh->mNumVertices;
		const unsigned _idxCount = currentMesh->mNumFaces;
		AssignVertexPositions(
			reinterpret_cast<const float*>(currentMesh->mVertices), 
			_vtxCount
		);

		AssignVertexNormals(
			reinterpret_cast<const float*>(currentMesh->mNormals),
			_vtxCount
		);



		std::vector<unsigned> indices;
		for (unsigned int i = 0; i < currentMesh->mNumFaces; ++i) {
			const aiFace& face = currentMesh->mFaces[i];
			// typically faces are triangles
			for (unsigned int j = 0; j < face.mNumIndices; ++j) {
				indices.push_back(face.mIndices[j]);
			}
		}


		AssignIndices(
			indices.data(),
			indices.size()
		);
	}

	

	// loading mats
	unsigned matCount	= scene->mNumMaterials;
	aiMaterial** matList = scene->mMaterials;
	

	

}

void Mesh::Init() {

	// init vertex positions
	//m_vertexPositions.assign(, );
	
	size_t vtxCount = sizeof(positionData) / (sizeof(positionData[0]) * 3);
	AssignVertexPositions(positionData, vtxCount);
	AssignVertexNormals(normalData, vtxCount);
	AssignIndices(idxList, sizeof(idxList) / sizeof(unsigned));
}






unsigned Mesh::GetVertexCount() const {
	//LOG_INFO("Vertex Count: "<< (sizeof(positionData) / 3 / sizeof(float)));
	return sizeof(positionData)/(3 * sizeof(float)); // pos = vec3 = 3x float
}


const float* Mesh::GetVertexData() const {
	return reinterpret_cast<const float*>(m_vertexPositions.data());
}


const size_t Mesh::GetVertexDataSize() const {
	return m_vertexPositions.size() * sizeof(glm::vec3);
}

const size_t Mesh::GetNormalDataSize() const {
	return m_vertexNormals.size() * sizeof(glm::vec3);
}
const float* Mesh::GetNormalData() const {
	return reinterpret_cast<const float*>(m_vertexNormals.data());
}

const size_t Mesh::GetUVDataSize() const {
	return sizeof(uvData);
}

const float* Mesh::GetUVData() const {
	return uvData;
}

const size_t Mesh::GetIndexDataSize() const {
	return m_indices.size() * sizeof(unsigned);
}

const unsigned* Mesh::GetIndexData() const {
	return m_indices.data();
}

const unsigned Mesh::GetIndexDataCount() const {
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
	m_vertexPositions.clear();
	m_vertexNormals.clear();
	m_vertexTangents.clear();
	m_uvs.clear();
	m_vertexColor.clear();
	m_indices.clear();
	m_boneWeights.clear();
	m_boneIndices.clear();
}

void Mesh::AssignVertexPositions(const float* _pointer, unsigned _vertexCount) {
	m_vertexPositions.resize(_vertexCount);
	for (unsigned i{}; i < _vertexCount; ++i) {
		unsigned offset{ i * 3 };
		m_vertexPositions[i] = glm::vec3{
			_pointer[offset + 0],
			_pointer[offset + 1],
			_pointer[offset + 2]
		};
	}

}

void Mesh::AssignVertexNormals(const float* _pointer, unsigned _vertexCount) {
	m_vertexNormals.resize(_vertexCount);
	for (unsigned i{}; i < _vertexCount; ++i) {
		unsigned offset{ i * 3 };
		m_vertexNormals[i] = glm::vec3{
			_pointer[offset + 0],
			_pointer[offset + 1],
			_pointer[offset + 2]
		};
	}
}

void Mesh::AssignIndices(const unsigned* _pointer, unsigned _indexCount) {	
	m_indices.assign(_pointer, _pointer + _indexCount);
}






