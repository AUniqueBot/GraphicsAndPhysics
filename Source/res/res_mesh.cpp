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

	Init();

	// general flow
	Assimp::Importer importer;
	std::string path{};
	const aiScene* scene = importer.ReadFile(
		path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType

	);


	if (!scene || !scene->HasMeshes()) return;
	
	
	unsigned meshCount	= scene->mNumMeshes;
	aiMesh** meshList	= scene->mMeshes;
	
	
	// loading models
	std::vector<std::vector<glm::vec3>> meshVertexList(meshCount);
	std::vector<std::vector<glm::vec3>> meshNormalsList(meshCount);
	std::vector<std::vector<glm::vec3>> meshUVsList(meshCount);
	for (unsigned meshIndex = 0; meshIndex < meshCount; ++meshIndex) {
		const aiMesh* currentMesh = meshList[meshIndex];

		std::vector<glm::vec3>& vertexListGLM	= meshVertexList[meshIndex];
		std::vector<glm::vec3>& normalsListGLM	= meshNormalsList[meshIndex];
		std::vector<glm::vec3>& uvListGLM		= meshUVsList[meshIndex];
		
		vertexListGLM.reserve(currentMesh->mNumVertices);
		normalsListGLM.reserve(currentMesh->mNumVertices);
		uvListGLM.reserve(currentMesh->mNumVertices);

		
		
		
		for (unsigned vtxIndex = 0; vtxIndex < currentMesh->mNumVertices; ++vtxIndex) {
			const aiVector3D& pos = currentMesh->mVertices[vtxIndex];
			const aiVector3D& nml = currentMesh->mNormals[vtxIndex];
			vertexListGLM.emplace_back(pos.x, pos.y, pos.z);
			normalsListGLM.emplace_back(nml.x, nml.y, nml.z);
		}
	}

	

	// loading mats
	unsigned matCount	= scene->mNumMaterials;
	aiMaterial** matList = scene->mMaterials;
	

	

}

void Mesh::Init() {}






unsigned Mesh::GetVertexCount() const {
	//LOG_INFO("Vertex Count: "<< (sizeof(positionData) / 3 / sizeof(float)));
	return sizeof(positionData)/(3 * sizeof(float)); // pos = vec3 = 3x float
}


const float* Mesh::GetVertexData() const {
	return positionData;
}


const size_t Mesh::GetVertexDataSize() const {
	return sizeof(positionData);
}

const size_t Mesh::GetNormalDataSize() const {
	return sizeof(normalData);
}
const float* Mesh::GetNormalData() const {
	return normalData;
}

const size_t Mesh::GetUVDataSize() const {
	return sizeof(uvData);
}

const float* Mesh::GetUVData() const {
	return uvData;
}

const size_t Mesh::GetIndexDataSize() const {
	return sizeof(idxList);
}

const unsigned* Mesh::GetIndexData() const {
	return idxList;
}

const unsigned Mesh::GetIndexDataCount() const {
	return sizeof(idxList)/sizeof(unsigned);
}


std::string Mesh::VAOIdentifier() const {
	return m_vaoName;
}

void Mesh::VAOIdentifier(std::string& _newIdentifier) {
	if (m_vaoName == _newIdentifier) return;
	m_vaoName = _newIdentifier;
}






