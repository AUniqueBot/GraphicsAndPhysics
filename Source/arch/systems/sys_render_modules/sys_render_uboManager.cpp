#include <arch/systems/sys_render_modules/sys_render_uboManager.h>

void UBO::Init() {
	if (m_bufferType == _COUNT) {
		LOG_ERROR("Unspecified Buffer Type. Ignoring.");
		return;
	}
	glGenBuffers(1, &m_bufferId);
	glBufferData(GL_UNIFORM_BLOCK, m_bufferSize, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(m_bufferType), m_bufferId);

	LOG_INFO("UBO initialized and bound. Buffer ID: {"  << m_bufferId <<"}");
}

size_t UBO::BufferSize() const {
	return m_bufferSize;
}

void UBO::BufferSize(size_t _size) {
	m_bufferSize = _size;
}



void UBO::BindBuffer() {
	glBindBuffer(GL_UNIFORM_BLOCK, m_bufferId);
}

void UBO::FillBufferData(const void* _data) {
	if (!_data) {
		LOG_ERROR("nullptr provided, no data filled");
	}
	BindBuffer();
	glBufferSubData(GL_UNIFORM_BLOCK, 0, m_bufferSize, _data);
}