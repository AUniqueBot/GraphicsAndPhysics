#pragma once
#include <pch.h>





enum ShadowMapType {
	TEXTURE = GL_TEXTURE_2D_ARRAY,
	CUBEMAP = GL_TEXTURE_CUBE_MAP_ARRAY
};

class ShadowMap {



public:
	
	void SetShadowMapType(ShadowMapType type);
	const ShadowMapType& GetShadowMapType() const;


	void SetFramebufferSize(glm::ivec2 _res);
	const glm::ivec2& GetFramebufferSize() const; 

	void SetBaseTileSize(glm::ivec2 _res);
	const glm::ivec2& GetBaseTileSize() const;

	void SetLayers(unsigned _layers);
	unsigned GetLayers() const;


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


private:

private:

	ShadowMapType m_shadowMapType			{ ShadowMapType::TEXTURE };

	glm::ivec2 m_framebufferSize			{ 4096, 4096 };
	glm::ivec2 m_baseTileSize				{ 2048, 2048 };	// 2048, 1024, 512, 256
	unsigned m_layers						{ 8 };
	unsigned m_fbo							{ 0 }; 
	unsigned m_shadowTextureId				{ 0 };
	unsigned m_levels						{ 4 };
	bool m_isBuilt							{};
	mutable unsigned m_currentBoundLayer	{ 0 };
	unsigned m_currentLayerCount			{ 0 }; 
	std::vector<unsigned> m_freeLayers		{ };
	

};