#include <arch/systems/sys_render_modules/sys_render_vaoManager.h>
#include <arch/resources/res_mesh.h>

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
	SetData("position", _mesh.GetVertexData(), _mesh.GetVertexDataSize());
	if (_mesh.GetNormalDataSize()) {
		SetData("normal", _mesh.GetNormalData(), _mesh.GetNormalDataSize());
	}
	if (_mesh.GetUVCount()) {
		SetData("uv", _mesh.GetUVData(0), _mesh.GetUVDataSize(0));
	}
	if (_mesh.GetIndexDataSize()) {
		SetVertexIndices(_mesh.GetIndexData(), _mesh.GetIndexDataSize());
	}
}




std::vector<GLuint> VAOHandler::GenerateBuffers(unsigned _bufferCount) {
	std::vector<unsigned> buffers;
	buffers.resize(_bufferCount);
	glGenBuffers(_bufferCount, buffers.data());
	return buffers;
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
	unsigned _dataSize, 
	GLenum _usage
) {
	if (!m_attributeBuffers.contains(_name)) {
		LOG_ERROR("Attempting to get invalid buffer: \'" << _name << '\'');
		return;
	}
	// get the buffer via

	const AttributeProps& attribute = m_attributeBuffers.at(_name);
	
	//LOG_SPLITTER();
	//LOG_INFO("Setting data " << _name);
	//LOG_INFO("size: " << _dataSize);
	//LOG_INFO("Setting data addr " << _data);
	//LOG_INFO("Setting data id: " << attribute.m_bufferID);
	//LOG_SPLITTER();

	//BindVAO();
	glBindBuffer(GL_ARRAY_BUFFER, attribute.m_bufferID);
	glBufferData(GL_ARRAY_BUFFER, _dataSize, _data, _usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//UnbindVAO();

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

