#pragma once
#include <arch/resources/res_resource.h>


class Mesh : public Resource {

private:

public:
	Mesh() : Resource{ MESH } {} //


	/// @brief loads the mesh into gpu memory.
	void Load();


		
	void AttachVBO();
	void AttachEBO();

	void UseVAO();



private:
	// mesh resource manages files

	// vertex thing.
	// VAO - vertex arch
	// VBO - vertex buffer
	// EBO - face for vtx

};
