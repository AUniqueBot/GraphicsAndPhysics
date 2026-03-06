
#include <util/util_logging.h>
#include <arch/systems/sys_render_modules/sys_render_uboManager.h>
#include <util/util_graphics_debugging.h>
#include <arch/components/comp_light.h>




void UBO::Init() {
	using namespace GraphicsDebug;

	//LOG_INFO("INIT START");
	glGenBuffers(1, &m_bufferId);
	GetError();
	if (!m_bufferId) {
		LOG_ERROR("BAD INDEX GENERATED");
	}
	else {

	}
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingIndex, m_bufferId);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	LOG_INFO("INIT END");
}


size_t UBO::BufferSize() const {
	return m_bufferSize;
}

void UBO::BufferSize(size_t _size) {
	m_bufferSize = _size;
}



void UBO::BindBuffer() const {
	if (m_currentBoundId == m_bufferId) return;
	m_currentBoundId = m_bufferId;

	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);	
	GraphicsDebug::GetError();
}

void UBO::SetBindingIndex(GLuint _bindingIndex) {
	if (_bindingIndex == m_bindingIndex) return;
	m_bindingIndex = _bindingIndex;
	if (m_bufferId) {
		glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingIndex, m_bufferId);
	}
}

void UBO::FillBufferData(const void* _data) const {
	if (!_data) {
		LOG_ERROR("nullptr provided, no data filled");
		return;
	}
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_bufferSize , _data);
	GraphicsDebug::GetError();
}


void UBO::UnbindBuffer() {
	if (m_currentBoundId == 0) return;
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_currentBoundId = 0;
}












void UBOManager::Init() {
		
}

void UBOManager::CreateUBO(BUFFER_TYPE _bufferType, size_t _size) {
	if (m_uboMap.contains(_bufferType)) {
		LOG_WARN("Buffer Type <" << typeid(_bufferType).name() << "> already created.");
		return;
	}
	UBO& ref = m_uboMap[_bufferType];
	ref.BufferSize(_size);
	ref.SetBindingIndex(static_cast<unsigned>(_bufferType));
	ref.Init();
}


UBO* UBOManager::GetUBO(BUFFER_TYPE _bufferType) {
	if (!m_uboMap.contains(_bufferType)) {
		return nullptr;
	}
	return &m_uboMap[_bufferType];
}

const UBO* UBOManager::GetUBO(BUFFER_TYPE _bufferType) const {
	if (!m_uboMap.contains(_bufferType)) {
		return nullptr;
	}
	return &m_uboMap.at(_bufferType);
}



