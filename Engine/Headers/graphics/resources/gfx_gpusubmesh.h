#pragma once
#include <arch/datatypes/type_sparseSet.h>

#include <graphics/resources/gfx_vao.h>
#include <graphics/resources/gfx_gpubuffer.h>


class Submesh;

class GPUSubmesh : public GPUVertexArrayObject {
public:
	void Load(const Submesh& _submesh);
	void Update();
	void Destroy() override;

	size_t GetIndexBufferElementCount() const;

private:
	void AttachIndexBuffer(const GPUBuffer& _buffer, size_t _indexBufferElementCount);
private:
	SparseSet<GLuint, GPUBuffer> m_vertexBuffers;
	GPUBuffer m_indexBuffer;
	size_t m_indexBufferElementCount;
};