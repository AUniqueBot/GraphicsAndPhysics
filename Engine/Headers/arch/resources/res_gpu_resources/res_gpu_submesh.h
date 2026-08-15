#pragma once
#include <arch/datatypes/type_sparseSet.h>
#include <arch/resources/res_mesh/res_mesh.h>
#include <arch/resources/res_gpu_resources/res_gpu_vao.h>
#include <arch/resources/res_gpu_resources/res_gpu_buffer.h>
#include <arch/resources/res_gpu_resources/res_gpu_resourceHandle.h>



class GPU_Submesh : public GPU_VertexArrayObject {
public:
	void Load(const Submesh& _submesh);
	void Destroy() override;
private:
	void AttachIndexBuffer(const GPU_Buffer&);
private:
	SparseSet<GLuint, GPU_Buffer> m_vertexBuffers;
	GPU_Buffer m_indexBuffer;
};