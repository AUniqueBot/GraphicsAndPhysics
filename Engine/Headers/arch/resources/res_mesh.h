#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>
#include <arch/systems/sys_render_modules/sys_render_vaoManager.h>


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

	void UploadModel();


protected:
	void ClearMeshInformation();


	void AssignVertexPositions(const float* _pointer, unsigned _vertexCount);
	void AssignVertexPositions(const glm::vec3* _pointer, unsigned _vertexCount);
	void AssignVertexNormals(const float* _pointer, unsigned _vertexCount);
	void AssignVertexNormals(const glm::vec3* _pointer, unsigned _vertexCount);
	void AssignIndices(const unsigned* _pointer, unsigned _indexGroupCount);
	void AssignIndices(const glm::uvec3* _pointer, unsigned _indexGroupCount);

	// - wip
	void AssignVertexUVs(const float* _pointer, unsigned _vertexCount);
	void AssignVertexColors(const float _pointer, unsigned _vertexCount);



protected:
	std::string m_vaoName				{ "vao_static" }; // vao identifier

	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<glm::vec4> m_vertexTangents;
	std::vector<std::vector<glm::vec2>> m_uvs;
	std::vector<glm::vec4> m_vertexColor;

	std::vector<glm::uvec3> m_indices;


	// - animation -----------------------
	std::vector<glm::vec4> m_boneWeights;
	std::vector<glm::ivec4> m_boneIndices;


	std::unordered_map<std::string, GLuint> m_bufferIDs;
};



