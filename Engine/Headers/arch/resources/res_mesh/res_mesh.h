#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_resourceHandle.h>
#include <arch/resources/res_mesh_vertexLayout.h>


// not including the whole of assimp here.
struct aiMesh;
using AttributeData = std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>>;
namespace MeshConstants {
	inline constexpr const char* C_VTXATTR_POSITION = "position";
	inline constexpr const char* C_VTXATTR_NORMAL = "normal";
	inline constexpr const char* C_VTXATTR_TANGENT = "tangent";
	inline constexpr const char* C_VTXATTR_COLOR = "color";
	inline constexpr const char* C_VTXATTR_UV = "uv";
	inline constexpr const char* C_VTXATTR_BONEWEIGHTS = "boneweights";
	inline constexpr const char* C_VTXATTR_BONEINDICES = "boneindices";
}


class Submesh {
	// this is an internal data storage for a mesh.
	// a mesh can contain multiple submeshes assuming a mesh takes more than 1 material.
public:
	Submesh() = default;
	Submesh(const Submesh&) = delete;
	Submesh& operator=(const Submesh&) = delete;

	Submesh(Submesh&&) noexcept = default;
	Submesh& operator=(Submesh&&) noexcept = default;


	

public:
	static Submesh CreateSubmesh(const aiMesh& _mesh, bool _isTriangulated);

	VertexAttributeDatabase* GetDatabase(const std::string& _name);
	const VertexAttributeDatabase* GetDatabase(const std::string& _name) const;
	AttributeData& GetVertexInformation();
	const AttributeData& GetVertexInformation() const;
	
	void ClearSubmeshInformation();

	// - attributes ----------------------------------------------------------------
	void SetVertexCount(size_t _vtxCount);
	size_t GetVertexCount() const;
	
public:
	// -- generic get/set --------------
	template <typename T>
	inline void SetData(const std::string& name, const T* _pointer, size_t _elementCount) {
		std::vector<T> data(_elementCount);
		data.assign(_pointer, _pointer + _elementCount);
		m_attributeData[name] = std::make_unique<VertexAttributeData<T>>(std::move(data));
	}

	template <typename T>
	inline const T* GetData(std::string _name) const {
		auto it = m_attributeData.find(_name);
		if (it == m_attributeData.end()) return nullptr;
		VertexAttributeData<T>* typedAttr = dynamic_cast<VertexAttributeData<T>*>(it->second.get());
		return typedAttr ? typedAttr->m_data.data() : nullptr; // pointer to underlying vector
	}

public:
	void SetVertexPositions(const glm::vec3* _pointer);
	const glm::vec3* GetVertexPositions() const;
	const size_t GetVertexDataSize() const;

	void SetVertexNormals(const glm::vec3* _pointer);
	const size_t GetNormalDataSize() const;
	const glm::vec3* GetNormalData() const;

	void SetVertexUVs(unsigned _index, const glm::vec2* _pointer);
	const size_t GetUVDataSize(unsigned _index) const;
	const glm::uvec2* GetUVData(unsigned _index) const;


	void SetVertexIndices(const glm::uvec3* _pointer, size_t _faceCount);
	const glm::uvec3* GetVertexIndexData() const;
	size_t GetVertexIndexCount() const;
public:
	const RES_ID& GetMaterialID() const;
	void SetMaterialID(const RES_ID& _id);

private:

	// is unique_ptr actually worth it here?
	size_t m_vertexCount				{ 0 };
	std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>> m_attributeData;
	std::vector<glm::uvec3> m_indices;

	RES_ID m_materialId					{ BaseResource::C_RES_ID_INVALID };
};

class Mesh : public Resource<Mesh> {

private:

public:


	/// @brief loads the mesh into gpu memory.
	void Load();

	virtual void Init();
	std::string ResourceTypeName() override { return "Mesh"; }



	void LoadAsset() override;
	void LoadMeshFromPath(std::filesystem::path _pathToModel);


	// - mesh data ----------------------------------
	size_t GetVertexCount() const;

	void SetSubmeshCount(size_t _count);
	size_t GetSubmeshCount() const;
	void AddSubmesh(Submesh&& _submesh);

	Submesh& GetSubmesh(int _idx);
	const Submesh& GetSubmesh(int _idx) const;
	std::vector<Submesh>& GetSubmeshList();
	const std::vector<Submesh>& GetSubmeshList() const;



	const size_t GetVertexDataSize() const;



	virtual const size_t GetIndexDataSize() const;
	virtual const glm::uvec3* GetIndexData() const;
	virtual const size_t GetIndexDataCount() const;



	std::string VAOIdentifier() const;
	void VAOIdentifier(std::string& _newIdentifier);



	void ClearMeshInformation();

	void SetVertexPositions(const glm::vec3* _pointer, size_t _vertexCount);
	void SetVertexNormals(const glm::vec3* _pointer, size_t _vertexCount);
	void SetIndices(const unsigned* _pointer, size_t _indexGroupCount);
	void SetIndices(const glm::uvec3* _pointer, size_t _indexGroupCount);

	
	VertexAttributeDatabase* GetDatabase(const std::string& _name);
	const VertexAttributeDatabase* GetDatabase(const std::string& _name) const;

	AttributeData& GetVertexInformation();
	const AttributeData& GetVertexInformation() const;



public:
	template <typename T>
	inline void SetData(const std::string& name, const T* _pointer, size_t _elementCount) {
		std::vector<T> data(_elementCount);
		data.assign(_pointer, _pointer + _elementCount);
		m_attributeData[name] = std::make_unique<VertexAttributeData<T>>(std::move(data));
	}



	template <typename T>
	inline const T* GetData(std::string _name) const {
		auto it = m_attributeData.find(_name);
		if (it == m_attributeData.end()) return nullptr;

		VertexAttributeData<T>* typedAttr = dynamic_cast<VertexAttributeData<T>*>(it->second.get());
		if (!typedAttr) return nullptr; // type mismatch

		return typedAttr->m_data.data(); // pointer to underlying vector
	}




protected:
	// - wip
	void SetVertexUVs(const float* _pointer, size_t _vertexCount);
	void SetVertexColors(const float _pointer, size_t _vertexCount);



protected:
	std::string m_vaoName							{ VAOConstants::C_VAO_STATIC_MESH }; // vao identifier

	std::vector<RES_ID> m_submeshMaterialIdentifier;
	std::vector<Submesh> m_submeshList;
	std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>> m_attributeData;

	std::vector<std::vector<glm::vec2>> m_uvs;
	std::vector<glm::vec4> m_vertexColor;

	std::vector<glm::uvec3> m_indices;


	// - animation -----------------------
	std::vector<glm::vec4> m_boneWeights;
	std::vector<glm::ivec4> m_boneIndices;
};


struct MeshHandle : public ResourceHandle {
	inline MeshHandle(ResourceIdentifierArg _resIdArg) : ResourceHandle(_resIdArg) {}
	inline std::shared_ptr<Mesh> GetMeshResource() {
		return GetResource<Mesh>();
	}
	inline std::shared_ptr<const Mesh> GetMeshResource() const {
		return GetResource<Mesh>();
	}
};



