#pragma once
#include <arch/resources/res_gpu_resources/res_gpu_vao.h>
#include <arch/resources/res_gpu_resources/res_gpu_buffer.h>

// unlike most GPU resources, VAO is
class GPU_Mesh {
public:


private:

	// stored in a manager, retrieved.
	GPU_VertexArrayObject m_handle;
	std::unordered_map<GLuint, GPU_Buffer> m_buffers;
};