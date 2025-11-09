#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>


/*
	class to handle render targets - where the gpu pastes it's render data to 
	basically render layers
*/
class RenderTarget	 {
public:
	RenderTarget() = default;
	~RenderTarget();

	void Create(glm::ivec2 _dimensions, unsigned numColorAttachments = 1, bool useDepth = true);
	void Destroy();


	void Resize(glm::ivec2 _newDimensions);
	void Resize(unsigned _width, unsigned _height);
	
	unsigned GetColorAttachment(unsigned index = 0) const;
	unsigned GetDepthAttachment() const;
	glm::ivec2 GetSize() const { return m_dimensions; }

	unsigned FBO() const;

	void Bind() const;
	void Unbind() const;
private:
	bool VerifyFBOCompleteness() const;
private:

	unsigned m_fbo						{}; // frame buffer identifier 
	std::vector<unsigned> m_fboColor	{};	// color attachments
	unsigned m_fboDepth					{}; // depth attachment

	bool m_isValid						{};

	std::string m_renderTargetName;
	glm::ivec2 m_dimensions	{};
};