#include <arch/systems/sys_render_modules/sys_render_shadowMap.h>




void ShadowMap::SetFramebufferSize(glm::ivec2 _res) {
	if (_res != m_framebufferSize) {
		m_framebufferSize = _res;

		if (m_isBuilt && m_textureHandle.HandleIsValid()) {
			Destroy();
			BuildShadowMap();
		}
	}
	
}

const glm::ivec2& ShadowMap::GetFramebufferSize() const {
	return m_textureHandle.HandleIsValid() ? m_textureHandle.GetDimensions() : m_framebufferSize;
}

void ShadowMap::SetBaseTileSize(glm::ivec2 _res) {
	m_baseTileSize = _res;
}

const glm::ivec2& ShadowMap::GetBaseTileSize() const {
	return m_baseTileSize;
}

void ShadowMap::SetLayers(unsigned _layers) {
	if (_layers == m_layers) return;
	m_layers = _layers;
	Destroy();
	BuildShadowMap();
}

unsigned ShadowMap::GetLayers() const {
	return m_layers;
}


void ShadowMap::SetupTextureArray(GLuint _handle) {


	glTextureParameteri(_handle, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	// comparison function (usually LEQUAL for depth)
	glTextureParameteri(_handle, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// 3. Set to LINEAR to enable 4-tap Hardware PCF
	glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1,1,1,1 };
	glTextureParameterfv(_handle, GL_TEXTURE_BORDER_COLOR, borderColor);

	GLint compareMode = 0;
	glGetTextureParameteriv(
		_handle,
		GL_TEXTURE_COMPARE_MODE,
		&compareMode
	);


}


void ShadowMap::BuildShadowMap() {
	
	if (!m_textureHandle.HandleIsValid()) return;
	GLuint shadowTexture = m_textureHandle.GetTextureHandle();
	SetupTextureArray(shadowTexture);
	glCreateFramebuffers(1, &m_fbo);
	glNamedFramebufferDrawBuffer(m_fbo, GL_NONE);
	glNamedFramebufferReadBuffer(m_fbo, GL_NONE);
	glNamedFramebufferTexture(
		m_fbo,
		GL_DEPTH_ATTACHMENT,
		shadowTexture,
		0
	);

	GLenum status = glCheckNamedFramebufferStatus(
		m_fbo,
		GL_FRAMEBUFFER
	);
	m_isBuilt = status == GL_FRAMEBUFFER_COMPLETE;
	if (!m_isBuilt) {

		switch (status) {
		case GL_FRAMEBUFFER_COMPLETE:
			// Framebuffer is valid and complete.
			break;

		case GL_FRAMEBUFFER_UNDEFINED:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_UNDEFINED");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_UNSUPPORTED");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
			break;

		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			LOG_ERROR("Framebuffer incomplete: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
			break;

		default:
			LOG_ERROR("Framebuffer incomplete: Unknown status [" << status << "]");
			break;
		}



		Destroy();
	}
}

const bool& ShadowMap::IsBuilt() const {
	return m_isBuilt;
}


void ShadowMap::Destroy() {
	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}

	m_isBuilt = false;
}



void ShadowMap::Bind() const {
	if (!m_textureHandle.HandleIsValid()) return;
	glm::ivec2 framebufferSize = m_textureHandle.GetDimensions();
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, framebufferSize.x, framebufferSize.y);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void ShadowMap::SetBoundLayer(unsigned _layer) const {
	GLuint shadowTexture = m_textureHandle.GetTextureHandle();
	assert(_layer < m_layers);
	if (m_currentBoundLayer == _layer) return;
	m_currentBoundLayer = _layer;
	glNamedFramebufferTextureLayer(m_fbo, GL_DEPTH_ATTACHMENT, shadowTexture, 0, _layer);
}


void ShadowMap::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned ShadowMap::FBO() const { 
	return m_fbo; 
}

unsigned ShadowMap::GetTextureID() const {
	return m_textureHandle.HandleIsValid() ? 
		m_textureHandle.GetTextureHandle() : 
		TextureConstants::C_INVALID_TEXTURE_ID;
}

bool ShadowMap::ValidateID(unsigned _id) const {
	return _id < m_layers;
}


unsigned ShadowMap::GenerateLayerID() {
	if (m_freeLayers.size() > 0) {
		unsigned freeId = m_freeLayers.back();
		m_freeLayers.pop_back();
		return freeId;
	}
	if (m_layers > m_currentLayerCount) {
		++m_shadowUsageCount;
		return m_currentLayerCount++;
	};
	LOG_WARN("No free slots available; assigning invalid ID (max unsigned value)");
	return std::numeric_limits<unsigned>::max();
}

void ShadowMap::ReclaimID(unsigned _id) {
	if (!ValidateID(_id)) return;
	--m_shadowUsageCount;
	m_freeLayers.push_back(_id);
}

bool ShadowMap::HasFreeLayers() const {
	return m_currentLayerCount < m_layers || m_freeLayers.size() > 0;
}

unsigned ShadowMap::GetLODLevels() const {
	return m_levels;
}

void ShadowMap::SetLODLevels(unsigned _levels) {
	m_levels = _levels;
}

void ShadowMap::SetTexture(const Texture2DArrayHandle& _info) {
	m_textureHandle = _info;
}

int ShadowMap::GetShadowMapUsageCount() const {
	return m_shadowUsageCount;
}




