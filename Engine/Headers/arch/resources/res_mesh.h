#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>
#include <arch/systems/sys_render_modules/sys_render_vaoManager.h>
#include <arch/resources/res_mesh_vertexLayout.h>

class Mesh : public Resource<Mesh> {

private:

public:


	/// @brief loads the mesh into gpu memory.
	void Load();

	virtual void Init();

	void LoadAsset() override;
	void LoadMeshFromPath(std::filesystem::path _pathToModel);


	// - mesh data ----------------------------------
	unsigned GetVertexCount() const;
	



	virtual const size_t GetVertexDataSize() const;
	virtual const float* GetVertexData() const;
	virtual const size_t GetNormalDataSize() const;
	virtual const float* GetNormalData() const;
	virtual const unsigned GetUVCount() const;
	virtual const size_t GetUVDataSize(unsigned _index) const;
	virtual const float* GetUVData(unsigned _index) const;
	virtual const size_t GetIndexDataSize() const;
	virtual const glm::uvec3* GetIndexData() const;
	virtual const size_t GetIndexDataCount() const;



	std::string VAOIdentifier() const;
	void VAOIdentifier(std::string& _newIdentifier);



	void ClearMeshInformation();

public:
	template <typename T>
	void SetData(const std::string& name, const T* _pointer, unsigned _elementCount) {
		std::vector<T> data(_elementCount);
		for (unsigned i = 0; i < _elementCount; ++i) {
			data[i] = _pointer[i];
		}
		m_attributeData[name] = std::make_unique<VertexAttributeData<T>>(std::move(data));
		m_attributeData[name];
	}



	template <typename T>
	const T* GetData(std::string _name) const {
		auto it = m_attributeData.find(_name);
		if (it == m_attributeData.end()) return nullptr;

		VertexAttributeData<T>* typedAttr = dynamic_cast<VertexAttributeData<T>*>(it->second.get());
		if (!typedAttr) return nullptr; // type mismatch

		return typedAttr->m_data.data(); // pointer to underlying vector
	}



protected:
	void SetVertexPositions(const float* _pointer, unsigned _vertexCount);
	void SetVertexNormals(const float* _pointer, unsigned _vertexCount);
	void SetIndices(const unsigned* _pointer, unsigned _indexGroupCount);
	void SetIndices(const glm::uvec3* _pointer, unsigned _indexGroupCount);

	// - wip
	void SetVertexUVs(const float* _pointer, unsigned _vertexCount);
	void SetVertexColors(const float _pointer, unsigned _vertexCount);



protected:
	std::string m_vaoName							{ "StaticMesh" }; // vao identifier

	std::vector<int> m_meshes;
	std::unordered_map<std::string, std::unique_ptr<VertexAttributeDatabase>> m_attributeData;

	std::vector<std::vector<glm::vec2>> m_uvs;
	std::vector<glm::vec4> m_vertexColor;

	std::vector<glm::uvec3> m_indices;


	// - animation -----------------------
	std::vector<glm::vec4> m_boneWeights;
	std::vector<glm::ivec4> m_boneIndices;


	std::unordered_map<std::string, GLuint> m_bufferIDs;
};



