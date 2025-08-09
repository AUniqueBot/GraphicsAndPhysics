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

	void Init();




	// - mesh data ----------------------------------
	unsigned GetVertexCount() const;
	
	const size_t GetVertexDataSize() const;
	const float* GetVertexData() const;

	const size_t GetNormalDataSize() const;
	const float* GetNormalData() const;
	
	const size_t GetUVDataSize() const;
	const float* GetUVData() const;

	const size_t GetIndexDataSize() const;
	const unsigned* GetIndexData() const;
	const unsigned GetIndexDataCount() const;


	std::string VAOIdentifier() const;
	void VAOIdentifier(std::string& _newIdentifier);


private:
	std::string m_vaoName		{ "vao_static" }; // vao identifier
	

	/*
		for now the mesh renderer will use a mesh array.
	
		- aw
	*/
};



