#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_resourceHandle.h>
#include <arch/resources/res_mesh_vertexLayout.h>
#include <arch/resources/res_mesh/res_submesh.h>


// not including the whole of assimp here.
class MeshRes;
class GPUResourceManager;
using AttributeData = std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>>;



// if this is the resource, then the handles
class MeshRes : public Resource<MeshRes> {

private:

public:

	//Mesh() ;


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
	const GPUResourceHandle& GetGPUResourceHandle() const;


	bool InfoDirty() const;

public:
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
		if (!typedAttr) return nullptr; // type mismatch

		return typedAttr->m_data.data(); // pointer to underlying vector
	}




protected:
	// - wip
	void SetVertexUVs(const float* _pointer, size_t _vertexCount);
	void SetVertexColors(const float _pointer, size_t _vertexCount);

	GPUResourceHandle m_meshHandle;
protected:
	friend class MeshManager;
	friend class GPUResourceManager;
	void SetGPUResourceHandle(GPUResourceHandle _newHandle);
	bool HasValidGPUResourceHandle() const;

protected:
	friend class RenderSystem;
	void FlagInfoClean();

protected:
	std::string m_vaoName							{ VAOConstants::C_VAO_DEFAULT_MESH }; // vao identifier

	std::vector<RES_ID> m_submeshMaterialIdentifier;
	std::vector<Submesh> m_submeshList;
	std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>> m_attributeData;

	
	std::vector<glm::uvec3> m_indices;

protected:
	bool m_infoDirty		{ true };

};


struct MeshHandle : public ResourceHandle {
	using ResourceType = MeshRes;
	inline MeshHandle(ResourceIdentifierArg _resIdArg) : ResourceHandle(_resIdArg) {}
	inline std::shared_ptr<MeshRes> GetMeshResource() {
		return GetResource<MeshRes>();
	}
	inline std::shared_ptr<const MeshRes> GetMeshResource() const {
		return GetResource<MeshRes>();
	}
};


