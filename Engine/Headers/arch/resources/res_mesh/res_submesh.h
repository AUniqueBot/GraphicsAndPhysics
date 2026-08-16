#pragma once
#include <arch/resources/res_resource.h>
#include <arch/resources/res_mesh_vertexLayout.h>
#include <arch/resources/res_gpu_resources/res_gpu_resourceHandle.h>

struct aiMesh;
using AttributeData = std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>>;
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
		m_infoDirty = true;
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
	size_t GetVertexIndexSize() const;


public:
	const RES_ID& GetMaterialID() const;
	void SetMaterialID(const RES_ID& _id);

public:
	bool InfoDirty() const;

protected:
	friend class RenderSystem;
	void FlagInfoClean();

private:

	// is unique_ptr actually worth it here?
	size_t m_vertexCount{ 0 };
	std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>> m_attributeData;
	std::vector<glm::uvec3> m_indices;

	bool m_infoDirty	{ true };

	RES_ID m_materialId{ BaseResource::C_RES_ID_INVALID };
};
