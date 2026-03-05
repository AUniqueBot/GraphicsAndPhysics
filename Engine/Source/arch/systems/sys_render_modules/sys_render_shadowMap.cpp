#include <arch/systems/sys_render_modules/sys_render_shadowMap.h>



void ShadowMap::SetResolution(glm::ivec2 _res) {
	if (_res == m_resolution) return;
	m_resolution = _res;
	Destroy();
	Build();
}

glm::ivec2 ShadowMap::GetResolution() const {
	return m_resolution;
}

void ShadowMap::SetLayers(unsigned _layers) {
	if (_layers == m_layers) return;
	m_layers = _layers;
	Destroy();
	Build();
}

unsigned ShadowMap::GetLayers() const {
	return m_layers;
}

void ShadowMap::Build() {	
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_shadowTex);

	glTextureStorage3D(
		m_shadowTex,
		1,
		GL_DEPTH_COMPONENT32F,
		m_resolution.x,
		m_resolution.y,
		m_layers
	);

	glTextureParameteri(m_shadowTex, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

	// 2. Set the comparison function (usually LEQUAL for depth)
	glTextureParameteri(m_shadowTex, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// 3. Set to LINEAR to enable 4-tap Hardware PCF
	glTextureParameteri(m_shadowTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_shadowTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glClearTexImage(m_shadowTex, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTextureParameteri(m_shadowTex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_shadowTex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_shadowTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(m_shadowTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	float borderColor[] = { 1,1,1,1 };
	glTextureParameterfv(m_shadowTex, GL_TEXTURE_BORDER_COLOR, borderColor);

	glCreateFramebuffers(1, &m_fbo);

	glNamedFramebufferDrawBuffer(m_fbo, GL_NONE);
	glNamedFramebufferReadBuffer(m_fbo, GL_NONE);


}

void ShadowMap::Destroy() {
	if (m_fbo) {
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}

	if (m_shadowTex) {
		glDeleteTextures(1, &m_shadowTex);
		m_shadowTex = 0;
	}
}



void ShadowMap::Bind(unsigned layer) const {
	LOG_DEBUG("Binding to shadow map layer: "<< layer);
	assert(layer < m_layers);
	glNamedFramebufferTextureLayer(
		m_fbo,
		GL_DEPTH_ATTACHMENT,
		m_shadowTex,
		0,
		layer
	);

	GLenum status = glCheckNamedFramebufferStatus(m_fbo, GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, m_resolution.x, m_resolution.y);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}


void ShadowMap::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned ShadowMap::FBO() const { return m_fbo; }

unsigned ShadowMap::TextureID() const {
	return m_shadowTex;
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
	if (m_layers > m_currentLayerCount) return m_currentLayerCount++;
	LOG_WARN("No free slots available; assigning invalid ID (max unsigned value)");
	return std::numeric_limits<unsigned>::max();
}

void ShadowMap::ReclaimID(unsigned _id) {
	if (ValidateID(_id)) m_freeLayers.push_back(_id);
}

bool ShadowMap::HasFreeLayers() const {
	return m_currentLayerCount < m_layers || m_freeLayers.size() > 0;
}
