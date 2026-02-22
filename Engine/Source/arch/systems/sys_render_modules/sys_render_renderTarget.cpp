#include <util/util_logging.h>
#include <arch/systems/sys_render_modules/sys_render_renderTarget.h>





RenderTarget::~RenderTarget() {
	Destroy();
}


void RenderTarget::Resolution(unsigned _width, unsigned _height) {
	Resolution({_width, _height});
}

void RenderTarget::Resolution(glm::ivec2 _res) {
	m_resolution = _res;
	Resize(_res);
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

	for (const ColorAttachment& ca : m_colorAttachments) {

		ColorChannelCastData key{ ca.m_channelLayout, ca.m_scalarType };
		if (!C_COLOR_FORMAT_LOOKUPTABLE.contains(key)) {
			LOG_ERROR("Combination of types [" << ToString(key.layout) << ", " << ToString(key.scalarType) << "] Not Found");
			continue;
		}

		const GLFormat data = C_COLOR_FORMAT_LOOKUPTABLE.at(key);
		
		glBindTexture(GL_TEXTURE_2D, ca.m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, data.internalFormat, width, height, 0, data.format, data.scalarType, nullptr);
	}

	if (m_depthAttachment.m_channelLayout != DepthChannelLayout::NONE) {
		const unsigned depthAttachmentType =
			m_depthAttachment.m_channelLayout == DepthChannelLayout::DEPTH ? GL_DEPTH_ATTACHMENT : GL_DEPTH_STENCIL;

		const DepthChannelCastData key{ m_depthAttachment.m_channelLayout, m_depthAttachment.m_scalarType };
		if (C_DEPTH_FORMAT_LOOKUPTABLE.find(key) != C_DEPTH_FORMAT_LOOKUPTABLE.end()) {
			const GLFormat& data = C_DEPTH_FORMAT_LOOKUPTABLE.at(key);

			glBindTexture(GL_TEXTURE_2D, m_depthAttachment.m_textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, data.internalFormat, width, height, 0, data.format, data.scalarType, nullptr);
		}
	}



	//glBindTexture(GL_TEXTURE_2D, 0);
	m_isValid = VerifyFBOCompleteness();
	Unbind();
	if (!m_isValid) {
		LOG_ERROR("FBO Generated is incomplete; destroying");
		Destroy();
	}
	else {
		LOG_INFO("FBO Generated");
	}
}






void RenderTarget::Resize(unsigned _width, unsigned _height) {
	Resize({ _width, _height });
}



const ColorAttachment& RenderTarget::GetColorAttachment(unsigned _index) const {
	return m_colorAttachments[_index];
}

unsigned RenderTarget::GetColorAttachmentTextureID(unsigned _index) const {
	return m_colorAttachments[_index].m_textureId;
}

unsigned RenderTarget::GetDepthAttachmentTextureID() const {
	return m_fboDepth;
}


void RenderTarget::AddInitialColorAttachments() {
	// 01
	ColorAttachment outColor{
		"out_color",
		ColorChannelLayout::RGBA,
		ScalarType::UINT,
		TextureFilter::LINEAR,
		GL_NONE,
		true
	};

	// 02 
	ColorAttachment outObjectId{
		"out_objectId",
		ColorChannelLayout::RED,
		ScalarType::UINT,
		TextureFilter::LINEAR,
		GL_NONE,
		true
	};

	// must be added in sequence.
	AddColorAttachment(outColor);
	AddColorAttachment(outObjectId);
	
	
	LOG_INFO(m_colorAttachments.size());
}

bool RenderTarget::AddColorAttachment(ColorAttachment _colAttachment) {
	GLint maxColAttachments{};
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColAttachments);
	if (m_colorAttachments.size() >= maxColAttachments) {
		LOG_ERROR("Reached maximum number of color attachments");
		return false;
	}

	// assign it a location.
	_colAttachment.m_attachmentIndex = m_currentColorAttachmentCounter;
	m_colorAttachments.push_back(_colAttachment);
	++m_currentColorAttachmentCounter;
	return true;
}

bool RenderTarget::SetDepthAttachment(DepthAttachment _depthAttachment) {
	m_depthAttachment = _depthAttachment;
	return true;
}



bool RenderTarget::Build() {

	const unsigned width	{ static_cast<unsigned>(m_resolution.x) };
	const unsigned height	{ static_cast<unsigned>(m_resolution.y) };
	glGenFramebuffers(1, &m_fbo);
	Bind();
	const size_t colAttachCount	{ m_colorAttachments.size() };
	std::vector<unsigned> texBuffer(colAttachCount);
	glGenTextures(static_cast<GLsizei>(colAttachCount), texBuffer.data());
	for (unsigned i{}; i < colAttachCount; ++i) {

		// lookup 
		ColorAttachment& ca		{ m_colorAttachments[i] };
		ca.m_textureId = texBuffer[i];
		glBindTexture(GL_TEXTURE_2D, ca.m_textureId);
		
		ColorChannelCastData k{ ca.m_channelLayout, ca.m_scalarType };
		if (!C_COLOR_FORMAT_LOOKUPTABLE.contains(k)) {
			LOG_ERROR("Combination of types [" << ToString(k.layout) << ", " << ToString(k.scalarType) << "] Not Found");
			continue;
		}

		const GLFormat& data{ C_COLOR_FORMAT_LOOKUPTABLE.at(k) };

		glTexImage2D(
			GL_TEXTURE_2D, 0, 
			data.internalFormat,
			m_resolution.x, 
			m_resolution.y, 
			0,
			data.format,
			data.scalarType,
			nullptr
		);


		GLenum textureFilterMin	{static_cast<GLenum>(ca.m_filter)};
		GLenum textureFilterMag { textureFilterMin };

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilterMag);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, 
			GL_COLOR_ATTACHMENT0 + ca.m_attachmentIndex, 
			GL_TEXTURE_2D, 
			ca.m_textureId, 
			0
		);
	}

	// depth channel
	if (m_depthAttachment.m_channelLayout != DepthChannelLayout::NONE) {
		// do something idk.

		const unsigned depthAttachmentType = 
			m_depthAttachment.m_channelLayout == DepthChannelLayout::DEPTH ? GL_DEPTH_ATTACHMENT :
			m_depthAttachment.m_channelLayout == DepthChannelLayout::DEPTH_STENCIL ? GL_DEPTH_STENCIL_ATTACHMENT : 
			GL_DEPTH_ATTACHMENT;
		
		LOG_INFO("Building with depth" << (m_depthAttachment.m_channelLayout == DepthChannelLayout::DEPTH_STENCIL ? "and Stencil "  : " ") << "enabled");

		const DepthChannelCastData key{ m_depthAttachment.m_channelLayout, m_depthAttachment.m_scalarType };

		if (C_DEPTH_FORMAT_LOOKUPTABLE.find(key) != C_DEPTH_FORMAT_LOOKUPTABLE.end()) {
			const GLFormat& data = C_DEPTH_FORMAT_LOOKUPTABLE.at(key); 
			glGenTextures(1, &m_depthAttachment.m_textureId);
			glBindTexture(GL_TEXTURE_2D, m_depthAttachment.m_textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, data.internalFormat, width, height, 0, data.format, data.scalarType, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, depthAttachmentType, GL_TEXTURE_2D, m_depthAttachment.m_textureId, 0);
		}
	}
	else {
		LOG_INFO("Building with no depth");
	}


	// - set draw here ----------------------------------
	std::vector<unsigned> drawBuffers{};

	// fire and forget bunch
	drawBuffers.reserve(m_colorAttachments.size());

	for (const ColorAttachment& ca : m_colorAttachments) {
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + ca.m_attachmentIndex);
	}

	glDrawBuffers(
		static_cast<GLsizei>(drawBuffers.size()),
		drawBuffers.data()
	);

	
	m_isValid = VerifyFBOCompleteness();

	if (!m_isValid) {
		LOG_ERROR("FBO Generated is incomplete; destroying");
		Destroy();
	}
	else {
		LOG_INFO("FBO Generated");
	}

	Unbind();

	return m_isValid;
}

void RenderTarget::Destroy() {

	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}


	if (m_colorAttachments.size()) {
		std::vector<unsigned> textureIDs;
		for (const ColorAttachment& c : m_colorAttachments) {
			textureIDs.push_back(c.m_textureId);
		}
		glDeleteTextures(static_cast<GLsizei>(textureIDs.size()), textureIDs.data());
		m_colorAttachments.clear();
		textureIDs.clear();
	}
	if (m_depthAttachment.m_channelLayout != DepthChannelLayout::NONE) {

		glDeleteTextures(1, &m_depthAttachment.m_textureId);
		//m_fboDepth = 0;
	}
	m_currentColorAttachmentCounter = 0;	
	LOG_INFO("Destroyed render target");
}




unsigned RenderTarget::FBO() const {
	return m_fbo;
}

void RenderTarget::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void RenderTarget::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//LOG_INFO("Target Unbound");
}

unsigned RenderTarget::PickPixel(
	glm::vec2 _vec, 
	unsigned _colorAttachment, 
	glm::vec2 _pixelResolution
) const {
	if (_pixelResolution.x == 0 || _pixelResolution.y == 0) {
		LOG_WARN("Resoution provided is 0 on one or both dimensions");
		return 0;
	};

	Bind(); 
	const ColorAttachment& colAttach{ GetColorAttachment(_colorAttachment) };
	glReadBuffer(GL_COLOR_ATTACHMENT0 + colAttach.m_attachmentIndex);


	GLuint readBuffer{};

	glReadPixels(
		static_cast<GLint>(_vec.x), 
		static_cast<GLint>(_vec.y), 
		static_cast<GLsizei>(_pixelResolution.x), 
		static_cast<GLsizei>(_pixelResolution.y), 
		GL_RED_INTEGER,
		GL_UNSIGNED_INT,
		&readBuffer
		);
	Unbind();
	return readBuffer;
}

//std::unordered_map<std::string, ColorAttachment> RenderTarget::GetOutputs() const {
//	std::unordered_map<std::string, ColorAttachment> retMap;
//	retMap.reserve();
//	for (const ColorAttachment& ca : m_colorAttachments) {
//		retMap[ca.m_name] = ca;
//	}
//	return retMap;
//}

void RenderTarget::LogBindErrors() const {
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		switch (status) {
		case GL_FRAMEBUFFER_UNDEFINED:           std::cerr << "GL_FRAMEBUFFER_UNDEFINED\n"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n"; break;
		case GL_FRAMEBUFFER_UNSUPPORTED:        std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED\n"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n"; break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n"; break;
		default: std::cerr << "Unknown FBO error\n"; break;
		}
	}

}


bool RenderTarget::VerifyFBOCompleteness() const {
	bool framebufferComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	if (!framebufferComplete) {
		LOG_ERROR("Framebuffer incomplete generation");
	}
	return framebufferComplete;
}



// -------------------------------------------------------
