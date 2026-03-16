
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

const GLuint& UBO::GetBindingIndex() const {
	return m_bindingIndex;
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


void UBOManager::CreateUBO(std::string _bufferName, int _bindIndex, size_t _size) {
	if (m_uboDatabase.At(_bufferName)) { 
		LOG_WARN("Buffer <" << _bufferName << "> already created.");
		return; 
	}
	UBO newUbo;
	newUbo.BufferSize(_size);
	newUbo.SetBindingIndex(_bindIndex);
	newUbo.Init();
	m_uboDatabase.Add(std::move(newUbo), _bufferName);
}

UBO* UBOManager::GetUBO(std::string _bufferName) {
	SparseSetView<UBO> data{ m_uboDatabase.At(_bufferName) };
	if (!data) return nullptr;
	return &*data;
}

const UBO* UBOManager::GetUBO(std::string _bufferName) const {
	SparseSetView<const UBO> data{ m_uboDatabase.At(_bufferName) };
	if (!data) return nullptr;
	return &*data;
}




void UBOManager::RemoveUBO(std::string _bufferName) {
	m_uboDatabase.Remove(_bufferName);
}

bool UBOManager::HasUBO(std::string _bufferName) const {
	return static_cast<bool>(m_uboDatabase.At(_bufferName));
}



