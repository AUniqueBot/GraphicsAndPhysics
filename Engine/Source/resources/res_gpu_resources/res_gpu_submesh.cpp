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
		buffer.Allocate(attr->DataSize(), GL_STATIC_DRAW);
		buffer.Upload(attr->Data(), attr->DataSize());
		
		// - hook up buffers ---
		GLuint bufferId = AliasToBinding(attrName);
		AttachBuffer(bufferId, buffer, attr->DatatypeSize());
		m_vertexBuffers.Add(std::move(buffer), bufferId);
	}
	// - set up 
	GPU_Buffer ebo;
	ebo.Create();
	size_t eboSize = _submesh.GetVertexIndexSize();
	ebo.Allocate(eboSize, GL_STATIC_DRAW);
	ebo.Upload(_submesh.GetVertexIndexData(), eboSize);
	AttachIndexBuffer(ebo);
}

void GPU_Submesh::Destroy() {
	for (GPU_Buffer& buffer: m_vertexBuffers) {
		buffer.Destroy();
	}
	m_vertexBuffers.clear();
	m_indexBuffer.Destroy();
	GPU_VertexArrayObject::Destroy();
}

void GPU_Submesh::AttachIndexBuffer(const GPU_Buffer& _buffer) {
	glVertexArrayElementBuffer(m_handle.Get(), _buffer.GetHandle());
	m_indexBuffer = _buffer;
}
