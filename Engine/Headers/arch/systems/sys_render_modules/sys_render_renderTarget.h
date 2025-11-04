#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>



class RenderTarget	 {
public:
	RenderTarget() = default;
	~RenderTarget();

	void Create(glm::ivec2 _dimensions, unsigned numColorAttachments = 1, bool useDepth = true);
	void Destroy();


	void Resize(glm::ivec2 _newDimensions);
	void Resize(unsigned _width, unsigned _height);
	
	void CreateTexture(unsigned& texID, GLenum internalFormat, GLenum format, GLenum type);


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