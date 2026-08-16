#include <arch/resources/res_gpu_resources/res_gpu_submesh.h>

void GPU_Submesh::Load(const Submesh& _submesh) {

	LOG_INFO("Make sure you already set up the vertex array!");
	// creation of the VAO; call VertexArrayObject::Create()
	Create();

	Clear();
	SetupAttributes(VertexLayouts::C_DEFAULT_MESH);

	
	const AttributeData& attrs = _submesh.GetVertexInformation();
	for (const auto& [attrName, attr] : attrs) {

		// - create buffers ----
		GPU_Buffer buffer;
		buffer.Create();
		buffer.Allocate(attr->DataSize(), GL_DYNAMIC_STORAGE_BIT);
		buffer.Upload(attr->Data(), attr->DataSize());
		
		// - hook up buffers ---
		GLuint bufferId = AliasToBinding(attrName);
		AttachBuffer(bufferId, buffer, attr->DatatypeSize());
		m_vertexBuffers.Add(std::move(buffer), bufferId);
		EnableAttribute(AliasToAttribute(attrName));
	}
	// - set up ebo --------------------
	GPU_Buffer ebo;
	ebo.Create();
	size_t eboSize = _submesh.GetVertexIndexSize();
	ebo.Allocate(eboSize, GL_DYNAMIC_STORAGE_BIT);
	ebo.Upload(_submesh.GetVertexIndexData(), eboSize);
	AttachIndexBuffer(ebo, _submesh.GetVertexIndexCount() * glm::uvec3::length());
}

void GPU_Submesh::Update() {

}

void GPU_Submesh::Destroy() {
	for (GPU_Buffer& buffer: m_vertexBuffers) {
		buffer.Destroy();
	}
	m_vertexBuffers.clear();
	m_indexBuffer.Destroy();
	GPU_VertexArrayObject::Destroy();
}

size_t GPU_Submesh::GetIndexBufferElementCount() const {
	return m_indexBufferElementCount;
}

void GPU_Submesh::AttachIndexBuffer(const GPU_Buffer& _buffer, size_t _elementCount) {
	LOG_INFO("Using buffer [" << _buffer.GetHandle() << "] as an index buffer.");
	glVertexArrayElementBuffer(m_handle.Get(), _buffer.GetHandle());
	m_indexBuffer = _buffer;
	m_indexBufferElementCount = _elementCount;

	GLint ebo;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);

	LOG_INFO("EBO bound to: [" << ebo << "]... " << (ebo == (GLint)_buffer.GetHandle()));
}
