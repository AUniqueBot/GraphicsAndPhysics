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
	glm::ivec2 m_resolution	{ 2048, 2048 };
	unsigned m_layers		{ 64 };
	unsigned m_fbo			{};
	unsigned m_shadowTex	{};


	unsigned m_currentLayerCount{}; 
	std::vector<unsigned> m_freeLayers{};
};