#include <arch/systems/sys_render_modules/sys_render_vaoManager.h>
#include <arch/resources/res_mesh.h>






void AttributeProps::SetAttributeEnabled(bool _isEnabled) {
	if (_isEnabled == m_isActive) return;
	m_isActive = _isEnabled;
	if (m_isActive) {
		glEnableVertexAttribArray(m_bindingPosition);
	}
	else {
		glDisableVertexAttribArray(m_bindingPosition);
	}
}

bool AttributeProps::GetAttributeEnabled() const {
	return m_isActive;
}



// - vaohandler ----------------------------------------------------------------

void VAOHandler::Init() {
	glGenVertexArrays(1, &m_vao);
}

void VAOHandler::BindVAO() const {
	//LOG_INFO("Binding Buffer " << m_vao); /// correct.
	glBindVertexArray(m_vao);
}

void VAOHandler::UnbindVAO() {
	glBindVertexArray(0);
}

void VAOHandler::SetVAO(unsigned _vao) {
	m_vao = _vao;
}

unsigned VAOHandler::GetVAO() const {
	return m_vao; 
}

void VAOHandler::UseMesh(const Mesh& _mesh) {
	if (!_mesh.GetVertexDataSize()) return;
	SetData("position", _mesh.GetVertexData(), _mesh.GetVertexCount(), sizeof(glm::vec3));
	SetData("normal", _mesh.GetNormalData(), _mesh.GetVertexCount(), sizeof(glm::vec3));
	SetData("uv", _mesh.GetUVData(0), _mesh.GetVertexCount(), sizeof(glm::vec2));
	SetVertexIndices(_mesh.GetIndexData(), _mesh.GetIndexDataSize());
}




std::vector<GLuint> VAOHandler::GenerateBuffers(unsigned _bufferCount) {
	std::vector<unsigned> buffers;
	buffers.resize(_bufferCount);
	glGenBuffers(_bufferCount, buffers.data());
	return buffers;
}

void VAOHandler::SetData(std::string _name, const Mesh& _mesh) {
	if (!m_attributeBuffers.contains(_name)) {
		// LOG_WARN();
		return;
	}
	AttributeProps& attr = m_attributeBuffers.at(_name);

	// load mesh to data

}


void VAOHandler::SetAttribute(
	std::string _name,
	GLuint _bindingPosition,
	GLuint _buffer,
	GLenum _type,
	unsigned _featureCount,
	bool _normalised
) {
	//BindVAO();
	//LOG_INFO("Adding Attribute - " << _name);
	AttributeProps& attr = m_attributeBuffers[_name];
	attr.m_name = _name;
	attr.m_bindingPosition = _bindingPosition;
	attr.m_bufferID = _buffer;
	attr.m_type = _type;
	attr.m_featureCount = _featureCount;
	attr.m_normalised = _normalised;
	attr.m_isActive = true;

	const int normalised = _normalised ? GL_TRUE : GL_FALSE;
	glBindBuffer(GL_ARRAY_BUFFER, _buffer);
	glVertexAttribPointer(_bindingPosition, _featureCount, _type, normalised, 0, (void*)0);
	glEnableVertexAttribArray(_bindingPosition);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//UnbindVAO();
}

unsigned VAOHandler::AttributeCount() const {
	return m_attributeBuffers.size();
}

void VAOHandler::LogDebug() const {
	LOG_SPLITTER();
	LOG_INFO("no of attributes: " << AttributeCount());
	for (const auto& [name, attr] : m_attributeBuffers) {
		LOG_INFO("attribute name: " << name);
		LOG_INFO("pos: " << attr.m_bindingPosition);
		LOG_INFO("bufferId: " << attr.m_bufferID);
		LOG_INFO("featureCount: " << attr.m_featureCount);
		LOG_INFO("isNormalised: " << attr.m_normalised);
	}
	
	LOG_SPLITTER();
}







void VAOHandler::SetData(
	std::string _name, 
	const void* _data, 
	unsigned _dataCount,
	unsigned _typeSize,
	GLenum _dataUsage
) {
	if (!m_attributeBuffers.contains(_name)) {
		LOG_ERROR("Attempting to get invalid buffer: \'" << _name << '\'');
		return;
	}
	// get the buffer via

	AttributeProps& attribute = m_attributeBuffers.at(_name);
	const unsigned pos = attribute.m_bindingPosition;
	
	bool isEnabled = _data != nullptr && _dataCount > 0;
	attribute.SetAttributeEnabled(isEnabled);
	if (isEnabled) {
		FillVertexAttributeData(
			attribute.m_bufferID,
			_data,
			_dataCount,
			_typeSize,
			_dataUsage
		);
	}
	else {
		FillEmptyVertexAttributeData(
			attribute.m_bindingPosition,
			_typeSize
		);
	}
}

void VAOHandler::FillVertexAttributeData(
	unsigned _bufferID,
	const void* _data, 
	unsigned _dataCount, 
	unsigned _typeSize, 
	GLenum _dataUsage
) {
	size_t dataSize { _dataCount * _typeSize };
	glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
	glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, _dataUsage);
	glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, _data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VAOHandler::FillEmptyVertexAttributeData(
	unsigned _attribPosition, 
	unsigned _typeSize
) {
	switch (_typeSize) {
		case sizeof(float) : 
			glVertexAttrib1f(_attribPosition, 0.0f); 
			break;
		case sizeof(glm::vec2) : 
			glVertexAttrib2f(_attribPosition, 0.0f, 0.0f); 
			break;
		case sizeof(glm::vec3) : 
			glVertexAttrib3f(_attribPosition, 0.0f, 0.0f, 0.0f); 
			break;
		case sizeof(glm::vec4) : 
			glVertexAttrib4f(_attribPosition, 0.0f, 0.0f, 0.0f, 0.0f); 
			break;
		default: 
			break; // optional error handling
	}
}



void VAOHandler::CreateEBO() {
	if (!m_ebo) glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void VAOHandler::SetVertexIndices(const void* _data, unsigned _dataSize) {
	//LOG_INFO(_data);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _dataSize, _data, GL_STATIC_DRAW);
}


// - vaomanager ----------------------------------------------------------------


void VAOManager::Init() {
	VAOHandler staticVAO;
	//LOG_INFO("Initialising Static VAO");

	staticVAO.Init();
	staticVAO.BindVAO();
	staticVAO.CreateEBO();
	// - setting up attributes --------------------
	std::vector<GLuint> vboBuffers = VAOHandler::GenerateBuffers(3);
	staticVAO.SetAttribute(
		"position", 0, 
		vboBuffers[0], 
		GL_FLOAT, 
		3, 
		GL_FALSE
	);
	staticVAO.SetAttribute(
		"normal", 
		1, 
		vboBuffers[1], 
		GL_FLOAT, 
		3, 
		GL_FALSE
	);
	staticVAO.SetAttribute(
		"uv", 
		2, 
		vboBuffers[2], 
		GL_FLOAT, 
		2, 
		GL_FALSE
	);

	AddVAO("vao_static", staticVAO);
	UnbindVAO();
}





void VAOManager::BindVAO(const std::string& _name) const {
	if (!m_vaoMap.contains(_name)) {
		// do note here
		return;
	}

	// log if needed.
	m_vaoMap.at(_name).BindVAO();
}

void VAOManager::UnbindVAO() const {
	glBindVertexArray(0);
}


void VAOManager::AddVAO(const std::string& _name, const VAOHandler& _vao) {
	//LOG_INFO("Adding a new VAO " << _name);
	if (m_vaoMap.contains(_name)) {
		LOG_WARN("VAO: "<< _name << " exists, overwriting");
	}
	m_vaoMap[_name] = _vao;
}


void VAOManager::RemoveVAO(std::string _name) {
	if (!m_vaoMap.contains(_name)) {
		LOG_INFO("VAO: " << _name << " doesn't exist");
		return; 
	}
	m_vaoMap.erase(_name);
}


VAOHandler* VAOManager::GetVAO(const std::string& _name) {
	auto it = m_vaoMap.find(_name);
	return (it == m_vaoMap.end()) ? nullptr : &it->second;
}

const VAOHandler* VAOManager::GetVAO(const std::string& _name) const {
	auto it = m_vaoMap.find(_name);
	return (it == m_vaoMap.end()) ? nullptr : &it->second;
}


