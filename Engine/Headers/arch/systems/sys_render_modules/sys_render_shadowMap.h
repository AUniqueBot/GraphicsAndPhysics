#pragma once
#include <pch.h>

class ShadowMap {

public:
	
	void SetResolution(glm::ivec2 _res);
	glm::ivec2 GetResolution() const;
	void SetLayers(unsigned _layers);
	unsigned GetLayers() const;


	void Build();
	void Destroy();


	void Bind(unsigned _layer) const;
	void Unbind() const;


	unsigned FBO() const;
	unsigned TextureID() const;
	 

	bool ValidateID(unsigned _id) const;
	unsigned GenerateLayerID();
	void ReclaimID(unsigned _id);
	bool HasFreeLayers() const;

private:

private:
	glm::ivec2 m_framebufferSize	{ 4096, 4096 };
	glm::ivec2 m_tileSize			{ 4096, 4096 };
	unsigned m_layers				{ 8 };
	unsigned m_fbo					{ 0 };
	unsigned m_shadowTex			{ 0 };


	unsigned m_currentLayerCount{}; 
	std::vector<unsigned> m_freeLayers{};
};