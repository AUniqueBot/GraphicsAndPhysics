#include <arch/resources/res_gpu_resources/res_gpu_vao.h>


void GPU_VertexArrayObject::Bind() const {
	glBindVertexArray(m_handle.Get());
}

void GPU_VertexArrayObject::Create() {
	if (!m_handle.IsValid()) {
		glCreateVertexArrays(1, &m_handle.Get());
	}
}

void GPU_VertexArrayObject::Destroy() {
	Clear();
	if ((GLuint)m_handle) {
		glDeleteVertexArrays(1, &m_handle.Get());
	}
}

void GPU_VertexArrayObject::SetupAttributes(const VertexLayout& _layout) {
	Clear();
	for (const VertexAttributeDesc& attr : _layout.attributes) {
		SetAttribute(attr.m_attributeSlot, attr.m_type, attr.m_featureCount, attr.m_normalized, attr.m_offset);
		SetBinding(attr.m_attributeSlot, attr.m_bindingSlot);
		m_aliasToBindings[attr.m_name] = attr.m_bindingSlot;
		m_aliasToAttributes[attr.m_name] = attr.m_attributeSlot;
		DisableAttribute(attr.m_attributeSlot);
	}
}


void GPU_VertexArrayObject::SetBinding(GLuint _attributeIndex, GLuint _bindingIndex) {
	if (!m_usedAttributes.contains(_attributeIndex)) {
		LOG_WARN("Set Attribute first");
	}
	m_usedBindings.insert(_bindingIndex);
	glVertexArrayAttribBinding((GLuint)m_handle, _attributeIndex, _bindingIndex);
}

void GPU_VertexArrayObject::SetAttribute(
	GLuint _arrayIndex, 
	GLenum  _type, 
	int _componentCount, 
	bool _normalized, 
	int _offset
) {

	if (_type == GL_INVALID_ENUM) {
		LOG_ERROR("Invalid datatype provided. exiting.");
		return;
	}
	GLuint vao = (GLuint)m_handle;
	glEnableVertexArrayAttrib(vao, _arrayIndex);
	glVertexArrayAttribFormat(vao, _arrayIndex, _componentCount, _type, _normalized, _offset);
	m_usedAttributes.insert(_arrayIndex);
}

void GPU_VertexArrayObject::EnableAttribute(GLuint _attributeIndex) {
	GLuint vao = (GLuint)m_handle;
	glEnableVertexArrayAttrib(vao, _attributeIndex);
}
void GPU_VertexArrayObject::DisableAttribute(GLuint _attributeIndex) {
	GLuint vao = (GLuint)m_handle;
	glDisableVertexArrayAttrib(vao, _attributeIndex);
	glVertexAttrib4f(_attributeIndex, 0.0f, 0.0f, 0.0f, 1.0f);
}


void GPU_VertexArrayObject::AttachBuffer(GLuint _bindingSlot, const GPU_Buffer& _buffer, int _stride) {
	if (m_usedBindings.contains(_bindingSlot)) {
		LOG_WARN("Rebinding bind slot.");
	}
	BufferID buffId = (GLuint)_buffer.GetHandle();
	glVertexArrayVertexBuffer((GLuint)m_handle, _bindingSlot, buffId, 0, _stride);
	m_usedBindings.insert(_bindingSlot);
	// SOA, We assume data is tightly packed.
}

int GPU_VertexArrayObject::AliasToBinding(std::string _alias) const {
	auto itr = m_aliasToBindings.find(_alias);
	return itr != m_aliasToBindings.end() ? 
		static_cast<int>(itr->second) : -1;
}
int GPU_VertexArrayObject::AliasToAttribute(std::string _alias) const {
	auto itr = m_aliasToAttributes.find(_alias);
	return itr != m_aliasToAttributes.end() ?
		static_cast<int>(itr->second) : -1;
}

void GPU_VertexArrayObject::Clear() {
	GLuint vao = (GLuint)m_handle;

	for (GLuint bindIdx : m_usedBindings) {
		glVertexArrayVertexBuffer(vao, bindIdx, 0, 0, 0);
	}
	for (GLuint attrIdx : m_usedAttributes) {
		glDisableVertexArrayAttrib(vao, attrIdx);
	}
	
	m_usedAttributes.clear();
	m_usedBindings.clear();
}


void GPU_VertexArrayObject::UseVAO() {
	glBindVertexArray((GLuint)m_handle);
}


