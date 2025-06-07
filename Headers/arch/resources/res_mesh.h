#pragma once
#include <arch/resources/res_resource.h>


class Mesh : public Resource {

private:

public:
	Mesh() : Resource{ MESH } {} //


	/// @brief loads the mesh into gpu memory.
	void Load();

	void Init();

	void UseVAO() const; // use this if you want to specify you want to use this mesh.

	unsigned IndexCount() const { return m_indexCount; };

	unsigned VAO() const { return m_vao; }
	unsigned VBO() const { return m_vbo; }


private:
	// mesh resource manages files

	// vertex thing.
	// VAO - vertex arch
	// VBO - vertex buffer
	// EBO - face for vtx


	unsigned m_vao	{};
	unsigned m_vbo	{};
	unsigned m_ebo	{};


	/*
		for now the mesh renderer will use a mesh array.
	
		- aw

	
	*/

	unsigned m_indexCount{};




};
