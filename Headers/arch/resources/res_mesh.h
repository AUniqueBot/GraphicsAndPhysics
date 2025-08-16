#pragma once
#include <pch.h>
#include <arch/resources/res_resource.h>
#include <arch/systems/sys_render_modules/sys_render_vaoManager.h>


class Mesh : public Resource {

private:

public:
	Mesh() : Resource{ MESH } {} //


	/// @brief loads the mesh into gpu memory.
	void Load();

	virtual void Init();




	// - mesh data ----------------------------------
	unsigned GetVertexCount() const;
	
	const size_t GetVertexDataSize() const;
	const float* GetVertexData() const;

	const size_t GetNormalDataSize() const;
	const float* GetNormalData() const;
	
	const unsigned GetUVCount() const;
	const size_t GetUVDataSize(unsigned _index) const;
	const float* GetUVData(unsigned _index) const;

	const size_t GetIndexDataSize() const;
	const unsigned* GetIndexData() const;
	const unsigned GetIndexDataCount() const;


	std::string VAOIdentifier() const;
	void VAOIdentifier(std::string& _newIdentifier);


protected:
	void ClearMeshInformation();


	void AssignVertexPositions(const float* _pointer, unsigned _vertexCount);
	void AssignVertexNormals(const float* _pointer, unsigned _vertexCount);
	void AssignIndices(const unsigned* _pointer, unsigned _indexCount);

	// - wip
	void AssignVertexUVs(const float* _pointer, unsigned _vertexCount);
	void AssignVertexColors(const float _pointer, unsigned _vertexCount);

protected:
	std::string m_vaoName		{ "vao_static" }; // vao identifier
	

	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::vec3> m_vertexNormals;
	std::vector<glm::vec4> m_vertexTangents;
	std::vector<std::vector<glm::vec2>> m_uvs;
	std::vector<glm::vec4> m_vertexColor;

	std::vector<unsigned> m_indices;


	// - animation -----------------------
	std::vector<glm::vec4> m_boneWeights;
	std::vector<glm::ivec4> m_boneIndices;



};



