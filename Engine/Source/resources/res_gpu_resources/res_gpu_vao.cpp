#include <arch/resources/res_gpu_resources/res_gpu_vao.h>


void GPU_VertexArrayObject::Create() {
	glCreateVertexArrays(1, &m_handle.Get());

	// if an error has occured..
}

void GPU_VertexArrayObject::Destroy() {
	if ((GLuint)m_handle) {
		glDeleteVertexArrays(1, &m_handle.Get());
	}
}

void GPU_VertexArrayObject::SetBinding(GLuint _attributeIndex, GLuint _bindingIndex) {
	if (!m_usedAttributes.contains(_attributeIndex)) {
		LOG_WARN("Set Attribute first");
	}
	m_usedBindings.insert(_bindingIndex);
	glVertexArrayAttribBinding((GLuint)m_handle, _attributeIndex, _bindingIndex);
}

void GPU_VertexArrayObject::SetAttribute(GLuint _arrayIndex, Datatype _type, int _componentCount, bool _normalized, int _offset) {
	GLenum type =
		_type == Datatype::INT ? GL_INT :
		_type == Datatype::FLOAT ? GL_FLOAT :
		GL_INVALID_ENUM;


	if (type == GL_INVALID_ENUM) {
		LOG_ERROR("Invalid datatype provided. exiting.");
		return;
	}
	GLuint vao = (GLuint)m_handle;
	glEnableVertexArrayAttrib(vao, _arrayIndex);
	glVertexArrayAttribFormat(vao, _arrayIndex, _componentCount, _type, _normalized, _offset);
	m_usedAttributes.insert(_arrayIndex);
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

void GPU_VertexArrayObject::EnableAttribute(GLuint _attributeIndex) {
	glEnableVertexArrayAttrib(m_handle, _attributeIndex);
}

void GPU_VertexArrayObject::DisableAttribute(GLuint _attributeIndex) {
	glDisableVertexArrayAttrib(m_handle, _attributeIndex);
}

void GPU_VertexArrayObject::UseVAO() {
	glBindVertexArray((GLuint)m_handle);
}


