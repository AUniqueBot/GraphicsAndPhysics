
#include <pch.h>
#include <arch/resources/res_mesh/res_submesh.h>
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
		switch (_index) {
		case 0:
			return VertexAttributeConstants::C_VTXATTR_UV0;
		case 1:
			return VertexAttributeConstants::C_VTXATTR_UV1;
		case 2:
			return VertexAttributeConstants::C_VTXATTR_UV2;
		case 3:
			return VertexAttributeConstants::C_VTXATTR_UV3;
		case 4:
			return VertexAttributeConstants::C_VTXATTR_UV4;
		case 5:
			return VertexAttributeConstants::C_VTXATTR_UV5;
		case 6:
			return VertexAttributeConstants::C_VTXATTR_UV6;
		case 7:
			return VertexAttributeConstants::C_VTXATTR_UV7;
		default:
			return VertexAttributeConstants::C_VTXATTR_UV0;
		}
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
	return GetData<glm::vec3>(VertexAttributeConstants::C_VTXATTR_POSITION);
}

void Submesh::SetVertexPositions(const glm::vec3* _pointer) {
	// mesh assumes the vertex count provided is correct.
	SetData<glm::vec3>(VertexAttributeConstants::C_VTXATTR_POSITION, _pointer, m_vertexCount);
}

const size_t Submesh::GetVertexDataSize() const {
	return m_attributeData.at(VertexAttributeConstants::C_VTXATTR_POSITION)->ElementCount() * sizeof(glm::vec3);
}



// -- normal ---------------
void Submesh::SetVertexNormals(const glm::vec3* _pointer) {
	SetData<glm::vec3>(VertexAttributeConstants::C_VTXATTR_NORMAL, _pointer, m_vertexCount);
}
const size_t Submesh::GetNormalDataSize() const {
	return m_attributeData.at(VertexAttributeConstants::C_VTXATTR_NORMAL)->ElementCount() * sizeof(glm::vec3);
}
const glm::vec3* Submesh::GetNormalData() const {
	return GetData<glm::vec3>(VertexAttributeConstants::C_VTXATTR_NORMAL);
}


// 
void Submesh::SetVertexUVs(unsigned _index, const glm::vec2* _pointer) {
	std::string uvId = GetUVVertexAttributeConstant(_index);
	SetData<glm::vec2>(uvId, _pointer, m_vertexCount);
}

const size_t Submesh::GetUVDataSize(unsigned _index) const {
	std::string uvId = GetUVVertexAttributeConstant(_index);
	return m_attributeData.at(uvId)->ElementCount() * sizeof(glm::uvec2);

}
const glm::uvec2* Submesh::GetUVData(unsigned _index) const {
	std::string uvId = GetUVVertexAttributeConstant(_index);
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

void Submesh::SetMaterialID(const RES_ID& _id) {
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
				std::string attrName = GetUVVertexAttributeConstant(id);
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