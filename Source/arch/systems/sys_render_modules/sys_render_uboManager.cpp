#include <arch/systems/sys_render_modules/sys_render_uboManager.h>
#include <util/util_graphics_debugging.h>


struct LightUBOData {
	int m_count{};
	int var2{};
	int _pad[2]{};
};


void UBO::Init() {
	auto GetError = GraphicsDebug::GetError;
	if (m_bufferType == _COUNT) {
		LOG_ERROR("Unspecified Buffer Type. Ignoring.");
		return;
	}
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	GraphicsDebug::GetError();
	glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, NULL, GL_DYNAMIC_DRAW);
	GetError();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_bufferId);
	GetError();

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::BufferType(BUFFER_TYPE _type) {
	m_bufferType = _type;
}

UBO::BUFFER_TYPE UBO::BufferType() const {
	return m_bufferType;
}

size_t UBO::BufferSize() const {
	return m_bufferSize;
}

void UBO::BufferSize(size_t _size) {
	m_bufferSize = _size;
}



void UBO::BindBuffer() {
	auto GetError = GraphicsDebug::GetError;
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);	
	GetError();

}

void UBO::FillBufferData(const void* _data) {
	auto GetError = GraphicsDebug::GetError;
	if (!_data) {
		LOG_ERROR("nullptr provided, no data filled");
		return;
	}
	BindBuffer();
	GetError();
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_bufferSize , _data);
	GetError();
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	GetError();
}













void UBOManager::Init() {
		
}

void UBOManager::CreateUBO(UBO::BUFFER_TYPE _bufferType, size_t _size) {
	if (m_uboMap.contains(_bufferType)) {
		LOG_WARN("Buffer Type <" << typeid(_bufferType).name() << "> already created.");
		return;
	}
	UBO& ref = m_uboMap[_bufferType];

	ref.BufferType(_bufferType);
	ref.BufferSize(_size);
	ref.Init();
}


UBO* UBOManager::GetUBO(UBO::BUFFER_TYPE _bufferType) {
	if (!m_uboMap.contains(_bufferType)) {
		return nullptr;
	}
	return &m_uboMap[_bufferType];
}

const UBO* UBOManager::GetUBO(UBO::BUFFER_TYPE _bufferType) const {
	if (!m_uboMap.contains(_bufferType)) {
		return nullptr;
	}
	return &m_uboMap.at(_bufferType);
}



