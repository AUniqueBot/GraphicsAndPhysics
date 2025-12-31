#include <util/util_logging.h>
#include <arch/systems/sys_render_modules/sys_render_renderTarget.h>





RenderTarget::~RenderTarget() {
	Destroy();
}

void RenderTarget::Create(glm::ivec2 _dimensions, unsigned numColorAttachments, bool useDepth) {
	m_dimensions = _dimensions;
	const unsigned width = m_dimensions.x, height = m_dimensions.y;
	glGenFramebuffers(1, &m_fbo);
	Bind();


	// - generate color attachments -------------------------------
	m_fboColor.resize(numColorAttachments);
	glGenTextures(numColorAttachments, m_fboColor.data());
	for (unsigned i = 0; i < numColorAttachments; ++i) {
		glBindTexture(GL_TEXTURE_2D, m_fboColor[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_fboColor[i], 0);
	}


	// - depth ----------------------------------------------------
	if (useDepth) {
		glGenTextures(1, &m_fboDepth);
		glBindTexture(GL_TEXTURE_2D, m_fboDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_fboDepth, 0);
	}


	// - draw buffers ---------------------------------------------

	std::vector<GLenum> drawBuffers{numColorAttachments};
	for (unsigned i{}; i < numColorAttachments; ++i) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(numColorAttachments, drawBuffers.data());

	m_isValid = VerifyFBOCompleteness();
	if (!m_isValid) {
		Destroy();
	}
	Unbind();
}

void RenderTarget::Destroy() {
	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}
	if (m_fboColor.size()) {
		glDeleteTextures(m_fboColor.size(), m_fboColor.data());
		m_fboColor.clear();
	}
	if (m_fboDepth) {
		glDeleteTextures(1, &m_fboDepth);
		m_fboDepth = 0;
	}

}


void RenderTarget::Resize(glm::ivec2 _newDimensions) {
	if (!m_isValid) return;
	Bind();
	const unsigned width = _newDimensions.x, height = _newDimensions.y;
	// resizes all textures created....
	for (const unsigned colorAttachment : m_fboColor) {
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	}

	if (m_fboDepth) {
		glBindTexture(GL_TEXTURE_2D, m_fboDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	//glBindTexture(GL_TEXTURE_2D, 0);
	m_isValid = VerifyFBOCompleteness();
	Unbind();
	if (!m_isValid) {
		Destroy();
	}
}



void RenderTarget::Resize(unsigned _width, unsigned _height) {
	Resize({ _width, _height });
}

unsigned RenderTarget::GetColorAttachment(unsigned index) const {
	return m_fboColor[index];
}

unsigned RenderTarget::GetDepthAttachment() const {
	return m_fboDepth;
}

//void RenderTarget::CreateTexture(unsigned& texID, GLenum internalFormat, GLenum format, GLenum type) {
//	glGenTextures(1, &texID);
//	glBindTexture(GL_TEXTURE_2D, texID);
//	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_dimensions.x, m_dimensions.y, 0, format, type, nullptr);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//}

//void RenderTarget::DestroyTexture(unsigned& texID) {
//	// stub function for now
//}

unsigned RenderTarget::FBO() const {
	return m_fbo;
}

void RenderTarget::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	LOG_INFO("Target Bound");
}

void RenderTarget::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	LOG_INFO("Target Unbound");
}

bool RenderTarget::VerifyFBOCompleteness() const {
	bool framebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	if (!framebufferComplete) {
		LOG_ERROR("Framebuffer incomplete generation");
	}
	return framebufferComplete;
}
