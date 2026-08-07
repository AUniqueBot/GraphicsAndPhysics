#pragma once
#include <pch.h>
#include <arch/resources/res_texture/res_texture.h>
#include <arch/resources/res_texture/res_texture2darray.h>





class ShadowMap {



public:

	void SetFramebufferSize(glm::ivec2 _res);
	const glm::ivec2& GetFramebufferSize() const; 

	void SetBaseTileSize(glm::ivec2 _res);
	const glm::ivec2& GetBaseTileSize() const;

	void SetLayers(unsigned _layers);
	unsigned GetLayers() const;

	void SetupTextureArray(GLuint _handle);


	void BuildShadowMap();
	const bool& IsBuilt() const;
	void Destroy();


	void Bind() const;
	void SetBoundLayer(unsigned _layer) const;
	static void Unbind();


	unsigned FBO() const;
	unsigned GetTextureID() const;
	 

	bool ValidateID(unsigned _id) const;
	unsigned GenerateLayerID();
	void ReclaimID(unsigned _id);
	bool HasFreeLayers() const;

	unsigned GetLODLevels() const;
	void SetLODLevels(unsigned _levels);



	void SetTexture(const Texture2DArray& _tex);
	//const Texture2DArray& GetTexture() const;


	const GLuint GetTextureHandle() const;
private:

private:
	glm::ivec2 m_framebufferSize			{ 2048, 2048};
	glm::ivec2 m_baseTileSize				{ 1024, 1024 };	// 2048, 1024, 512, 256
	unsigned m_layers						{ 8 };
	unsigned m_fbo							{ 0 }; 
	unsigned m_levels						{ 4 };
	bool m_isBuilt							{ false };
	mutable unsigned m_currentBoundLayer	{ 0 };
	unsigned m_currentLayerCount			{ 0 }; 
	std::vector<unsigned> m_freeLayers		{ };
	
	// reference to texture in tex manager; to be created when render system is init.
	Texture2DArray m_textureHandle			{ std::nullopt }; // needs a proper constructor.
	bool m_initialized						{ false };


};