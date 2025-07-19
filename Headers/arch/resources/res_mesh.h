#pragma once
#include <pch.h>
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



private:
	// mesh resource manages files

	// vertex thing.
	// VAO - vertex arch
	// VBO - vertex buffer
	// EBO - face for vtx


	unsigned m_vao		{};
	unsigned m_vbo[3]	{};
	unsigned m_ebo		{};
	std::vector<GLuint> m_vboList{};

	/*
		for now the mesh renderer will use a mesh array.
	
		- aw

	
	*/

	unsigned m_indexCount{};




};
